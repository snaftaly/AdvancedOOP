/*
 * 305220980A.h
 *
 *  Created on: Apr 11, 2016
 *      Author: noaleibo1
 */

#ifndef _305220980_C_H_
#define _305220980_C_H_

#include <string>
#include <map>
#include <cstdlib>
#include <vector>
#include "Sensor.h"
#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include <stack>

class _305220980_C: public AbstractAlgorithm {

	const AbstractSensor * sensor;
	std::map<std::string, int> configs;
	std::stack<Direction> previousSteps;
	int stepsUntillFinishing;

public:

	_305220980_C();
	~_305220980_C();

	void setSensor(const AbstractSensor& sensor);
	void setConfiguration(std::map<std::string, int> config);
	Direction step();
	void aboutToFinish(int stepsTillFinishing);
	Direction getStep();
};

#endif /* 305220980C_H_ */
