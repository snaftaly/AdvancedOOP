#include <queue>
#include <map>
#include <iostream>
#include "SmartGenericAlgorithm.h"

using namespace std;

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
	prevStepFromAlgo = Direction::Stay;
}

void SmartGenericAlgorithm::setConfiguration(std::map<std::string, int> config){
	batteryMng.setBatteryCapacity(config["BatteryCapacity"]);
	batteryMng.setBatteryRechargeRate(config["BatteryRechargeRate"]);
	batteryMng.setBatteryConsumptionRate(config["BatteryConsumptionRate"]);
	maxStepsAfterWinner = config["MaxStepsAfterWinner"];
	batteryMng.resetBattery();
}

void SmartGenericAlgorithm::aboutToFinish(int stepsTillFinishing){
	stepsUntillFinishing = stepsTillFinishing;
}


void SmartGenericAlgorithm::updateAlgorithmInfo(Direction lastStep){
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
    tempCellInfo.dirt = (s.dirtLevel != 0)? s.dirtLevel : 0;
    tempCellInfo.stepsToDocking = stepsFromDocking;
    tempCellInfo.isWall = false;
    houseMapping[currPosition] = tempCellInfo;

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
	// if robot is in dock then stay, else go to the neighbor closest to dock that is not a wall which has the max dirt
	Direction dirClosestToDock = Direction::Stay;
	if (!isRobotInDock()){
		int bestDistanceFromDock = stepsFromDocking;
		int bestDirtFromDock = 0;
		for(Direction d: directions) {
			Point neighbour = currPosition;
			neighbour.move(d);
			auto neighbourInfo = houseMapping.find(neighbour);
			if(neighbourInfo != houseMapping.end()) {
				CellInfo cellinfo = neighbourInfo->second;
				if(!cellinfo.isWall && cellinfo.stepsToDocking != -1 &&
//						cellinfo.stepsToDocking < bestDistanceFromDock){
						(cellinfo.stepsToDocking < bestDistanceFromDock ||
						(cellinfo.stepsToDocking == bestDistanceFromDock && cellinfo.dirt > bestDirtFromDock))) {
					bestDistanceFromDock = cellinfo.stepsToDocking;
					bestDirtFromDock = cellinfo.dirt;
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
			Point neighbour = currPoint;
			neighbour.move(d);
			if(parent.find(neighbour) == parent.end()) { //don't add neighbor to queue if it was already visited
				auto neighbourInfo = houseMapping.find(neighbour);
				if(neighbourInfo == houseMapping.end() || !neighbourInfo->second.isWall) {
					// neighbor is not yet mapped, or is mapped and is not a wall
//					cout << "adding neighbor" << endl;
			        parent[neighbour] = {currPoint, d};
			        Q.push(neighbour);
				}
			}
	    }
	    Q.pop(); // remove the head from the queue
	}

	if (Q.empty()){ // this means we didn't find any unrevealed/dirty position - so go back to docking
//		cout << " Q is empty" << endl;
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


Direction SmartGenericAlgorithm::getStepAndUpdatePrevStep(const std::vector<Direction>& possibleMoves,
		Direction stepFromSimulator, bool useCaution){

	float cautionRate = useCaution ? 1.1 : 1.0;
	directions = possibleMoves;
    updateAlgorithmInfo(stepFromSimulator);

    Direction nextStep =  Direction::Stay; // default direction is stay

	if (isRobotInDock() && !batteryMng.isBatteryFull()){
		// robot is in docking station and battery not full - stay in place
		nextStep = Direction::Stay;
//		cout << "stay in dock" << endl;
	}
	else {
		int batteryToGetToDockingForStep = (stepsFromDocking+1)*batteryMng.getBatteryConsumptionRate();

		// Check if the robot needs to head back to the docking station - either because:
		// - aboutToFinish is called and there are not enough steps
		// - battery is not enough to go back
		// - about to finish was not called but there might not be enough step (algo using caution)
		if((stepsUntillFinishing != -1 && stepsUntillFinishing <= stepsFromDocking+1) ||
				batteryMng.getBatteryState() <= batteryToGetToDockingForStep*cautionRate ||
				(useCaution && maxStepsAfterWinner <= stepsFromDocking)){
			nextStep = getDirectionClosestToDocking(); // get the direction that has the minimum steps to docking
//			cout << "robot needs to head back" << endl;

		}
		// The robot doesn't have to go back - so stay if still dirty or go to closest unrevealed place
		else {
			if (s.dirtLevel > 0){ // place is still dirty
				nextStep = Direction::Stay;
//				cout << "robot don't need to head back, but place is dirty" << endl;

			}
			else { // current place is clean so search for the closest unrevealed/dirty place
				nextStep = getDirectionToClosestNeededPlace();
//				cout << "robot don't need to head back and place is clean" << endl;
			}
		}
	}
	return nextStep;
}

bool SmartGenericAlgorithm::isRobotInDock(){
	return (currPosition.getX() == 0 && currPosition.getY() == 0);
}
