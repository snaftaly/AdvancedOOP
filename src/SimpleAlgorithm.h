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

	// need to use AbstractSensor * http://moodle.tau.ac.il/mod/forum/discuss.php?d=44675#p68447
	const AbstractSensor * sensor;
	std::map<std::string, int> configs;

public:

	SimpleAlgorithm();
	~SimpleAlgorithm();

	void setSensor(const AbstractSensor& sensor);
	void setConfiguration(std::map<std::string, int> config);
	Direction step();
	void aboutToFinish(int stepsTillFinishing);
	std::list<int> getPossibleSteps()


};

#endif /* SIMPLEALGORITHM_H_ */
