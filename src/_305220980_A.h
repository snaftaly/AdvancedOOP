/*
 * 305220980A.h
 *
 *  Created on: Apr 11, 2016
 *      Author: noaleibo1
 */

#ifndef _305220980_A_H_
#define _305220980_A_H_

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

class _305220980_A: public AbstractAlgorithm {

	const AbstractSensor * sensor;
	std::map<std::string, int> configs;
	std::stack<Direction> previousSteps;
	int stepsUntillFinishing;
	BatteryManager batteryMng;

public:

	_305220980_A();
	~_305220980_A();

	void setSensor(const AbstractSensor& sensor);
	void setConfiguration(std::map<std::string, int> config);
	Direction step();
	void aboutToFinish(int stepsTillFinishing);
	Direction getStep();
};

#endif /* 305220980A_H_ */
