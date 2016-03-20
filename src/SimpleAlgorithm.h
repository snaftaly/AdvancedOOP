/*
 * simpleAlgorithm.h
 *
 *  Created on: Mar 12, 2016
 *      Author: noaleibo1
 */

#ifndef SIMPLEALGORITHM_H_
#define SIMPLEALGORITHM_H_

#include <string>
#include <map>
#include <iostream>
#include <cstdlib>
#include "Sensor.h"
#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"

class SimpleAlgorithm: public AbstractAlgorithm {

	Sensor sensor; //TODO: Check why can't be AbstractSensor.
	std::map<std::string, int> configs;

public:

	SimpleAlgorithm();
	~SimpleAlgorithm();

	void setSensor(const Sensor& s);
	void setConfiguration(std::map<std::string, int> config);
	Direction step();
	void aboutToFinish(int stepsTillFinishing);


};

#endif /* SIMPLEALGORITHM_H_ */
