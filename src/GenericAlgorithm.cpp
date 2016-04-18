/*
 * GenericAlgorithm.cpp
 *
 *  Created on: Apr 18, 2016
 *      Author: noaleibo1
 */

#include "GenericAlgorithm.h"

GenericAlgorithm::GenericAlgorithm(): sensor(NULL), stepsUntillFinishing(-1) {
	// TODO Auto-generated constructor stub

}

GenericAlgorithm::~GenericAlgorithm() {
	// TODO Auto-generated destructor stub
}


void GenericAlgorithm::setSensor(const AbstractSensor& s){
	this->sensor = &s; // passing the pointer to sensor

	//Reset previous knowledge.
	stepsUntillFinishing = -1;

	//Empty stack of previous steps - TODO-Check.
	std::stack<Direction> empty;
	std::swap(previousSteps, empty);
}

void GenericAlgorithm::setConfiguration(std::map<std::string, int> config){
	configs = config;
	batteryMng.setConfiguration(config);
	batteryMng.setBatteryState(config["BatteryCapacity"]);
}


Direction GenericAlgorithm::step(){

	Direction nextStep = getStep();
	int batteryConsumptionRate = configs["BatteryConsumptionRate"];
	int batteryRechargeRate = configs["BatteryRechargeRate"];
	int batteryCapacity = configs["BatteryCapacity"];

	cout << "battery state!!!!" << batteryMng.getBatteryState() <<endl;//TODO delete

	if (previousSteps.empty() && batteryCapacity > batteryMng.getBatteryState()){
		batteryMng.updateBatteryState(batteryRechargeRate);
		if (batteryCapacity <= batteryMng.getBatteryState()){
			stepsUntillFinishing = -1;
		}
	}
	else{
		batteryMng.updateBatteryState(batteryConsumptionRate*(-1));
	}

	int batteryToGetToDocking = previousSteps.size()*batteryConsumptionRate;

	if (batteryMng.getBatteryState() <= batteryToGetToDocking){
		aboutToFinish(previousSteps.size());
	}

	return nextStep; //Direction::East;
}

void GenericAlgorithm::aboutToFinish(int stepsTillFinishing){
	stepsUntillFinishing = stepsTillFinishing;
}
