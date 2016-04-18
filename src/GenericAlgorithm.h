/*
 * GenericAlgorithm.h
 *
 *  Created on: Apr 18, 2016
 *      Author: noaleibo1
 */

#ifndef GENERICALGORITHM_H_
#define GENERICALGORITHM_H_

#include <string>
#include <map>
#include <cstdlib>
#include <vector>
#include "Sensor.h"
#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include <stack>
#include "BatteryManager.h"


class GenericAlgorithm: public AbstractAlgorithm {

	const AbstractSensor * sensor;
	std::map<std::string, int> configs;
	std::stack<Direction> previousSteps;
	int stepsUntillFinishing;
	BatteryManager batteryMng;

public:
	GenericAlgorithm();
	virtual ~GenericAlgorithm();

	void setSensor(const AbstractSensor& sensor);
	void setConfiguration(std::map<std::string, int> config);
	Direction step();
	void aboutToFinish(int stepsTillFinishing);
	virtual Direction getStep();
};

#endif /* GENERICALGORITHM_H_ */
