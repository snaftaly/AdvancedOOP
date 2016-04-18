/*
 * 305220980A.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: noaleibo1
 */

#include "_305220980_A.h"

_305220980_A::_305220980_A(): sensor(NULL), stepsUntillFinishing(-1) {
	// TODO Auto-generated constructor stub
}

_305220980_A::~_305220980_A() {
	// TODO Auto-generated destructor stub
}

void _305220980_A::setSensor(const AbstractSensor& s){
	this->sensor = &s; // passing the pointer to sensor

	//Reset previous knowledge.
	stepsUntillFinishing = -1;

	//Empty stack of previous steps - TODO-Check.
	std::stack<Direction> empty;
	std::swap(previousSteps, empty);
}

void _305220980_A::setConfiguration(std::map<std::string, int> config){
	configs = config;
	batteryMng.setConfiguration(config);
	batteryMng.setBatteryState(config["BatteryCapacity"]);
}


Direction _305220980_A::step(){

	Direction nextStep = getStep();
	int batteryConsumptionRate = configs["BatteryConsumptionRate"];
	int batteryRechargeRate = configs["BatteryRechargeRate"];

	cout << "battery state!!!!" << batteryMng.getBatteryState() <<endl;//TODO delete

	if (previousSteps.empty() && stepsUntillFinishing > 0){
		batteryMng.updateBatteryState(batteryRechargeRate);
		stepsUntillFinishing = -1;
	}
	else{
		batteryMng.updateBatteryState(batteryConsumptionRate*(-1));
	}

//	cout << "BatteyToGetHome" << previousSteps.size()*batteryConsumptionRate << endl;
//	cout << "batteryState" << batteryMng.getBatteryState()  << endl; TODO delete

	int batteryToGetToDocking = previousSteps.size()*batteryConsumptionRate;
	int batteryCapacity = configs["BatteryCapacity"];

	if (batteryMng.getBatteryState() <= batteryToGetToDocking && batteryMng.getBatteryState() < batteryCapacity){
		cout << "!!!about to finish" << endl;
		aboutToFinish(previousSteps.size());
	}

	return nextStep; //Direction::East;
}

Direction _305220980_A::getStep(){

	Direction nextStep;

	//The robot is heading back to the docking station.
	if(stepsUntillFinishing != -1){
		if (!previousSteps.empty()){
			nextStep = previousSteps.top();
			previousSteps.pop();
		}
		else{
			nextStep = Direction::Stay;
		}
	}

	else{
		int dirtLevel = sensor->sense().dirtLevel;
		if (dirtLevel > 0){
			nextStep = Direction::Stay;
		}

		else{
			std::vector<Direction> possibleMoves {Direction::East,Direction::North,Direction::South,Direction::West,Direction::Stay};
			Direction currDirection;
			int i = 0;
			while ((currDirection = possibleMoves[i]) != Direction::Stay && sensor->sense().isWall[(int)currDirection]){
				i++;
			}
			nextStep = currDirection;
		}

		//Adds only steps that are not 'stay'
		switch (nextStep){
			case Direction::East:
				previousSteps.push(Direction::West);
				break;
			case Direction::West:
				previousSteps.push(Direction::East);
				break;
			case Direction::North:
				previousSteps.push(Direction::South);
				break;
			case Direction::South:
				previousSteps.push(Direction::North);
				break;
			case Direction::Stay:
				break;
		}
	}

	return nextStep;
}

void _305220980_A::aboutToFinish(int stepsTillFinishing){
	stepsUntillFinishing = stepsTillFinishing;
}
