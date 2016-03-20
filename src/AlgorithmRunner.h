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
	int roboti, robotj;

	int batteryLevel;
	int stepsRemaining;
	int numSteps;
	int dirtCollected;

	bool isFinished;
	int algoRankInCompetition;

	std::list<int> housesScore;

	static map<string, int> config;
	static int currHouseTotDirt;
	static int currHouseDocki;
	static int currHouseDockj;

	bool isLegalStep(int stepi, int stepj);

public:
	AlgorithmRunner(AbstractAlgorithm* a);
	virtual ~AlgorithmRunner();

	void setCurrHouse(const House& currHouse) {
		this->currHouse = currHouse;
	}

	Sensor& getSensor(){
		return sensor;
	}

	void resetRunnerForNewHouse(const House& house);

	bool isHouseCleanAndRobotInDock();
	bool isBatteryConsumedAndRobotNotInDock();
	bool isNoMoreStepsToRun();
	bool getStepAndUpdateIfLegal();
	bool isBackInDocking();
	int getPositionInCompetition();


	void updateStepsRemainingOnWinner();
	void updateCurrHouseScoreInList(int winnerMaxSteps);

	// setters

	void setIsFinished(bool isFinished) {
		this->isFinished = isFinished;
	}

	void setAlgoRankInCompetition(int algoRankInCompetition) {
		this->algoRankInCompetition = algoRankInCompetition;
	}
	// getters

	bool getIsFinished() const {
		return isFinished;
	}

	const std::list<int>& getHousesScore() const {
		return housesScore;
	}

	int getStepsRemaining() const {
		return stepsRemaining;
	}

	// static functions
	static void AlgorithmRunner::resetCommonDataForNewHouse(const House& house);

	static void setConfig(const map<string, int>& config) {
		this->config = config;
	}


};

#endif /* ALGORITHMRUNNER_H_ */
