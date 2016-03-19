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
	Sensor sensor;

	// current house info:
	House currHouse;
	int houseTotalDirtLevel;
	int roboti, robotj;

	int batteryConsumed;
	int numOfSteps;
	int dirtCollected;

	int algoRankInCompetition;

	std::list<int> housesScore;
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

	void resetRunnerForNewHouse(const House& house);

	static void setConfig(const map<string, int>& config) {
		this->config = config;
	}
};

#endif /* ALGORITHMRUNNER_H_ */
