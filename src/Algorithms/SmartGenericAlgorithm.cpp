#include <queue>
#include <map>
#include "SmartGenericAlgorithm.h"

SmartGenericAlgorithm::SmartGenericAlgorithm(): sensor(NULL), stepsUntillFinishing(-1){
	// TODO Auto-generated constructor stub

}

SmartGenericAlgorithm::~SmartGenericAlgorithm() {
	// TODO Auto-generated destructor stub
}


void SmartGenericAlgorithm::setSensor(const AbstractSensor& s){
	this->sensor = &s; // passing the pointer to sensor
	//Reset previous knowledge.
	stepsUntillFinishing = -1;
	batteryMng.resetBattery();
//	isGoingBack = false;
	prevStepFromAlgo = Direction::Stay;
}

void SmartGenericAlgorithm::setConfiguration(std::map<std::string, int> config){
	batteryMng.setBatteryCapacity(config["BatteryCapacity"]);
	batteryMng.setBatteryRechargeRate(config["BatteryRechargeRate"]);
	batteryMng.setBatteryConsumptionRate(config["BatteryConsumptionRate"]);
	maxStepsAfterWinner = config["MaxStepsAfterWinner"];
}

void SmartGenericAlgorithm::aboutToFinish(int stepsTillFinishing){
	stepsUntillFinishing = stepsTillFinishing;
}


void SmartGenericAlgorithm::updateAlgorithmInfo(Direction lastStep){
//    ++totalSteps; // update total steps
    currPosition.move(lastStep); // update the robot position, as managed by the algorithm, to the new position
    isRobotInDock() ? batteryMng.chargeBattery(): batteryMng.consumeBattery(); // update battery state

	// update num steps remaining for next step
	if (stepsUntillFinishing != -1){
		stepsUntillFinishing--;
	}

    s = sensor->sense(); // sense the new location

    // update the map with the info on the dirt level
    stepsFromDocking = calcStepsToDocking(stepsFromDocking + 1, currPosition);
    CellInfo tempCellInfo;
    tempCellInfo.dirt = (s.dirtLevel != 0)? s.dirtLevel - 1 : 0;
    tempCellInfo.stepsToDocking = stepsFromDocking;
    tempCellInfo.isWall = false;
    houseMapping[currPosition] = tempCellInfo;
    // TODO: remove this
//    houseMapping[currPosition] = {(s.dirtLevel != 0)? s.dirtLevel - 1 : 0, stepsFromDocking}; // if we are in pos it's not a wall
    // update all 4 cells around me with Wall information and if possible also with stepsToDocking
    for(Direction d : directions) {
        Point checkPosition = currPosition;
        checkPosition.move(d);
        auto neighbour = houseMapping.find(checkPosition);
        if(neighbour == houseMapping.end()) {
            houseMapping[checkPosition].isWall = s.isWall[(int)d];
            if(!s.isWall[(int)d]) {
                houseMapping[checkPosition].stepsToDocking = calcStepsToDocking(stepsFromDocking + 1, checkPosition);
            }
        }
    }
}

int SmartGenericAlgorithm::calcStepsToDocking(int stepsFromDocking, const Point& position) {

    // if this cell already has a better route - keep it (it is already the best one)!
    auto thisCellInfo = houseMapping.find(position);
    if(thisCellInfo != houseMapping.end() && thisCellInfo->second.stepsToDocking != -1 &&
    		thisCellInfo->second.stepsToDocking < stepsFromDocking) {
    	return thisCellInfo->second.stepsToDocking;
    }
    // check if there is a better route via neighbours that we visited already
    int bestRouteSteps = stepsFromDocking;

    for(Direction d: directions) {
        Point pointToCheck = position;
        pointToCheck.move(d);
        auto neighbourInfo = houseMapping.find(pointToCheck);
        if(neighbourInfo != houseMapping.end() && neighbourInfo->second.stepsToDocking != -1 &&
           neighbourInfo->second.stepsToDocking < bestRouteSteps - 1) {
                bestRouteSteps = neighbourInfo->second.stepsToDocking + 1;
        }
    }
    if(bestRouteSteps < stepsFromDocking) {
        updateStepsToDocking(bestRouteSteps, position); // update current position and neighbors with the new distance
    }
    return bestRouteSteps;
}

void SmartGenericAlgorithm::updateStepsToDocking(int stepsToDocking, const Point& position) {
    if(houseMapping[position].stepsToDocking > stepsToDocking || houseMapping[position].stepsToDocking == -1) {
        houseMapping[position].stepsToDocking = stepsToDocking;
        for(Direction d: directions) {
            Point neighbour = position;
            neighbour.move(d);
            auto neighbourInfo = houseMapping.find(neighbour);
            if(neighbourInfo != houseMapping.end()) {
                CellInfo cell = neighbourInfo->second;
                if(cell.stepsToDocking > stepsToDocking + 1 || (!cell.isWall && cell.stepsToDocking == -1) ) {
                    updateStepsToDocking(stepsToDocking + 1, neighbour);
                }
            }
        }
    }
}

Direction SmartGenericAlgorithm::getDirectionClosestToDocking() {
	// if robot is in dock then stay, else go to the neighbor closest to dock that is not a wall
	Direction dirClosestToDock = Direction::Stay;
	if (!isRobotInDock()){
		int bestDistanceFromDock = stepsFromDocking;
		for(Direction d: directions) {
			Point neighbour = currPosition;
			neighbour.move(d);
			auto neighbourInfo = houseMapping.find(neighbour);
			if(neighbourInfo != houseMapping.end()) {
				CellInfo cellinfo = neighbourInfo->second;
				if(!cellinfo.isWall && cellinfo.stepsToDocking != -1 && cellinfo.stepsToDocking < bestDistanceFromDock) {
					bestDistanceFromDock = cellinfo.stepsToDocking;
					dirClosestToDock = d;
				}
			}
		}
	}
	return dirClosestToDock;
}

Direction  SmartGenericAlgorithm::getDirectionToClosestNeededPlace(){
	queue<Point> Q;
	map<Point, pair<Point, Direction>> parent; // maps between each point and it's parent and the direction from parent to child
	Q.push(currPosition); // a queue for the bfs, initalized with the current position
	parent[currPosition] = {currPosition, Direction::Stay};
	Point wantedPoint = currPosition;

	while (!Q.empty()){
	    Point currPoint = Q.front(); // get the head of the queue
		auto pointInfo = houseMapping.find(currPoint);
	    if (pointInfo == houseMapping.end() ||
	    		(!pointInfo->second.isWall && pointInfo->second.dirt != 0)){
	    	// point is not yet mapped OR
	    	// point is mapped and it is not a wall and the dirt level is unknown (-1) or dirt level > 0
	    	wantedPoint = currPoint;
	    	break;
	    }
	    for (Direction d: directions){
			Point neighbour = currPosition;
			neighbour.move(d);
			if(parent.find(neighbour) == parent.end()) { //don't add neighbor to queue if it was already visited or is
				auto neighbourInfo = houseMapping.find(neighbour);
				if(neighbourInfo == houseMapping.end() || !neighbourInfo->second.isWall) {
					// neighbor is not yet mapped, or is mapped and is not a wall
			        parent[neighbour] = {currPoint, d};
			        Q.push(neighbour);
				}
			}
	    }
	    Q.pop(); // remove the head from the queue
	}

	if (Q.empty()){ // this means we didn't find any unrevealed/dirty position - so go back to docking
		return getDirectionClosestToDocking();
	}
	else{ // we found an unrevealed/dirty place - get the direction that will take us there
		Point retPoint = wantedPoint;
		Direction retDirection = Direction::Stay;
		while (! (wantedPoint == parent[wantedPoint].first)){
			retPoint = wantedPoint;
			retDirection = parent[retPoint].second;
			wantedPoint = parent[wantedPoint].first;
		}
		return retDirection;

	}


}


Direction SmartGenericAlgorithm::getStepAndUpdatePrevStep(const std::vector<Direction>& possibleMoves, Direction stepFromSimulator){
	directions = possibleMoves;
    updateAlgorithmInfo(stepFromSimulator);

    Direction nextStep =  Direction::Stay; // default direction is stay

	if (isRobotInDock() && !batteryMng.isBatteryFull()){
		// robot is in docking station and battery not full - stay in place
		nextStep = Direction::Stay;
	}
	else {
		int batteryToGetToDockingForStep = (stepsFromDocking+1)*batteryMng.getBatteryConsumptionRate();

		// Check if the robot needs to head back to the docking station - either because:
		// - aboutToFinish is called and there are not enough steps
		// - battery is not enough to go back
		// - about to finish was not called but there might not be enough step (algo using caution)
		if((stepsUntillFinishing != -1 && stepsUntillFinishing <= stepsFromDocking+1) ||
				batteryMng.getBatteryState() <= batteryToGetToDockingForStep ||
				stepsFromDocking+1 > maxStepsAfterWinner ){
			nextStep = getDirectionClosestToDocking(); // get the direction that has the minimum steps to docking
		}
		// The robot doesn't have to go back - so stay if still dirty or go to closest unrevealed place
		else {
			if (s.dirtLevel > 0){ // place is still dirty
				nextStep = Direction::Stay;
			}
			else { // current place is clean so search for the closest unrevealed/dirty place
				nextStep = getDirectionToClosestNeededPlace();
			}
		}
	}
	return nextStep;

//	if (stepFromSimulator != prevStepFromAlgo || !isGoingBack){
//		// if the step from simulator is not what algo meant, we definitely have to add it to prevSteps (we are not going back for sure)
//		// if the step is equal (got it from simulator as well as from algo) and we are not going back then we also need to update it
//		updatePreviousStep(stepFromSimulator); //Step received from the simulator as previous step. EX. 3 modification.
//	}
//
//	// update robot distance from battery for prev step
//	updateXYfromDock(stepFromSimulator);
//
//	// update battery for prev step
//	if (isRobotInDock()){
//		batteryMng.chargeBattery();
//	}
//	else {
//		batteryMng.consumeBattery();
//	}
//
//	// update num steps remaining for next step
//	if (stepsUntillFinishing != -1){
//		stepsUntillFinishing--;
//	}
//
//

//	else {
//		int numPrevSteps = previousSteps.size();
//		int batteryToGetToDockingForStep = (numPrevSteps+1)*batteryMng.getBatteryConsumptionRate();
//		if((stepsUntillFinishing != -1 && stepsUntillFinishing <= numPrevSteps+1) ||
//				batteryMng.getBatteryState() <= batteryToGetToDockingForStep ||
//				numPrevSteps+1 > maxStepsAfterWinner ){
//			// The robot needs to head back to the docking station - either because:
//			// - aboutToFinish is called and there are not enough steps
//			// - battery is not enough to go back
//			// - about to finish was not called but there might not be enough step (algo using caution)
//			isGoingBack = true;
//			if (!previousSteps.empty()){ // there are steps to go back
//				nextStep = previousSteps.top();
//				previousSteps.pop();
//			}
//			else {  // there are no more steps to go back
//				nextStep = Direction::Stay;
//			}
//		}
//		else{ // the robot doesn't have to go back
//			isGoingBack = false;
//			int dirtLevel = sensor->sense().dirtLevel; // the current place is dirty so stay in it
//			if (dirtLevel > 0){
//				nextStep = Direction::Stay;
//			}
//			else{
//				for (Direction direction : possibleMoves){
//					if (!(direction == Direction::Stay) && !sensor->sense().isWall[(int)direction]){
//						nextStep = direction;
//						break;
//					}
//				}
//			}
//		}
//	}
//	prevStepFromAlgo = nextStep;
//	return nextStep;
}

bool SmartGenericAlgorithm::isRobotInDock(){
	return (currPosition.getX() == 0 && currPosition.getY() == 0);
}

//void SmartGenericAlgorithm::updatePreviousStep(const Direction & prevStep){
//	//Adds only steps that are not 'stay'
//	switch (prevStep){
//		case Direction::East:
//			previousSteps.push(Direction::West);
//			break;
//		case Direction::West:
//			previousSteps.push(Direction::East);
//			break;
//		case Direction::North:
//			previousSteps.push(Direction::South);
//			break;
//		case Direction::South:
//			previousSteps.push(Direction::North);
//			break;
//		case Direction::Stay:
//			break;
//	}
//}

//void SmartGenericAlgorithm::updateXYfromDock(const Direction & prevStep){
//	// update robot distance from battery for next step
//	switch (prevStep){
//		case Direction::East:
//			xDistanceFromDock++;
//			break;
//		case Direction::West:
//			xDistanceFromDock--;
//			break;
//		case Direction::North:
//			yDistanceFromDock++;
//			break;
//		case Direction::South:
//			yDistanceFromDock--;
//			break;
//		case Direction::Stay:
//			break;
//	}
//}
