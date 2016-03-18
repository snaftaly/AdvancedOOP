/*
 * AlgorithmRunner.h
 *
 *  Created on: Mar 17, 2016
 *      Author: noaleibo1
 */

#ifndef ALGORITHMRUNNER_H_
#define ALGORITHMRUNNER_H_
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include <list>

class AlgorithmRunner {
	AbstractAlgorithm* algorithm;
	House currHouse;
	Sensor sensor;
	int houseTotalDirtLevel;
	bool isRunning;
	//TODO refer to battery.
	std::list<int> housesScore;


public:
	AlgorithmRunner(AbstractAlgorithm* a);
	virtual ~AlgorithmRunner();
	//TODO isLegalStep()

	void setCurrHouse(const House& currHouse) {
		this->currHouse = currHouse;
	}
};

#endif /* ALGORITHMRUNNER_H_ */
