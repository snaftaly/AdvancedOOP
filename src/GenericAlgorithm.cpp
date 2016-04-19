#include "GenericAlgorithm.h"

GenericAlgorithm::GenericAlgorithm(): sensor(NULL), stepsUntillFinishing(-1), xDistanceFromDock(0), yDistanceFromDock(0){
	// TODO Auto-generated constructor stub

}

GenericAlgorithm::~GenericAlgorithm() {
	// TODO Auto-generated destructor stub
}


void GenericAlgorithm::setSensor(const AbstractSensor& s){
	this->sensor = &s; // passing the pointer to sensor

	//Reset previous knowledge.
	stepsUntillFinishing = -1;
	xDistanceFromDock = 0;
	yDistanceFromDock = 0;
	batteryMng.resetBattery();


	//Empty stack of previous steps - TODO-Check.
	std::stack<Direction> empty;
	std::swap(previousSteps, empty);
}

void GenericAlgorithm::setConfiguration(std::map<std::string, int> config){
	batteryMng.setBatteryCapacity(config["BatteryCapacity"]);
	batteryMng.setBatteryRechargeRate(config["BatteryRechargeRate"]);
	batteryMng.setBatteryConsumptionRate(config["BatteryConsumptionRate"]);
}

void GenericAlgorithm::aboutToFinish(int stepsTillFinishing){
	stepsUntillFinishing = stepsTillFinishing;
}


Direction GenericAlgorithm::getStep(const std::vector<Direction>& possibleMoves){

	cout << "battery state!!!!" << batteryMng.getBatteryState() << endl;//TODO delete
	Direction nextStep =  Direction::Stay; // default is stay


	if (isRobotInDock() && !batteryMng.isBatteryFull()){
		// robot is in docking station and battery not full - stay in place
		nextStep = Direction::Stay;
	}
	else {
		int numPrevSteps = previousSteps.size();
		int batteryToGetToDockingForStep = (numPrevSteps+1)*batteryMng.getBatteryConsumptionRate();
		if((stepsUntillFinishing != -1 && stepsUntillFinishing < numPrevSteps+1) ||
				batteryMng.getBatteryState() < batteryToGetToDockingForStep){
			// The robot needs to head back to the docking station - either because:
			// - aboutToFinish is called and there are not enough steps
			// - battery is not enough to go back
			if (!previousSteps.empty()){ // there are steps to go back
				nextStep = previousSteps.top();
				previousSteps.pop();
			}
			else {  // there are no more steps to go back
				nextStep = Direction::Stay;
			}
		}
		else{ // the robot doesn't have to go back
			int dirtLevel = sensor->sense().dirtLevel; // the current place is dirty so stay in it
			if (dirtLevel > 0){
				nextStep = Direction::Stay;
			}
			else{
				for (Direction direction : possibleMoves){
					if (!sensor->sense().isWall[(int)direction]){
						nextStep = direction;
						break;
					}
				}
			}
		}
	}

	//Adds only steps that are not 'stay' and update robot distance from battery for next step
	switch (nextStep){
		case Direction::East:
			xDistanceFromDock++;
			previousSteps.push(Direction::West);
			break;
		case Direction::West:
			xDistanceFromDock--;
			previousSteps.push(Direction::East);
			break;
		case Direction::North:
			yDistanceFromDock++;
			previousSteps.push(Direction::South);
			break;
		case Direction::South:
			yDistanceFromDock--;
			previousSteps.push(Direction::North);
			break;
		case Direction::Stay:
			break;
	}

	// update num steps remaining for next step
	if (stepsUntillFinishing != -1){
		stepsUntillFinishing--;
	}
	// update battery for next step
	if (isRobotInDock()){
		batteryMng.chargeBattery();
	}
	else {
		batteryMng.consumeBattery();
	}
	cout << "battery state = " << batteryMng.getBatteryState() << endl;//TODO delete

	return nextStep;
}

bool GenericAlgorithm::isRobotInDock(){
	return (xDistanceFromDock == 0 && yDistanceFromDock == 0);
}
