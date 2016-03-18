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
#include <tuple>
#include <map>
#include <string>

class AlgorithmRunner {
	AbstractAlgorithm* algorithm;
	House currHouse;
	Sensor sensor;
	int houseTotalDirtLevel;
	bool isRunning;
	//TODO refer to battery.
	std::list<int> housesScore;
	int numOfSteps;
	int dirtCollected;
	static map<string, int> config;


public:
	AlgorithmRunner(AbstractAlgorithm* a);
	virtual ~AlgorithmRunner();
	//TODO isLegalStep()

	void setCurrHouse(const House& currHouse) {
		this->currHouse = currHouse;
	}

	Sensor& getSensor(){
		return sensor;
	}

	void updateHouseAndInfo(const House& house);

	static void setConfig(const map<string, int>& config) {
		this->config = config;
	}
};

#endif /* ALGORITHMRUNNER_H_ */
