#ifndef ALGORITHMRUNNER_H_
#define ALGORITHMRUNNER_H_
#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "Sensor.h"
#include "SimulationFinishState.h"
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
	int numSteps;
	int dirtCollected;

	bool isFinished;
	int algoPositionInCompetition;
	SimulationFinishState finishState;

	std::list<int> housesScore;

	static map<string, int> config;

	// TODO: check if we can remove these static variables - just put the dirt data in each algo runner
	static int currHouseTotDirt;

	bool isLegalStep(int stepi, int stepj);

	void printSimulation(int stepi, int stepj);
public:

	AlgorithmRunner(AbstractAlgorithm* a);
	virtual ~AlgorithmRunner();

	void setCurrHouse(const House& currHouse) {
		this->currHouse = currHouse;
	}

	Sensor& getSensor(){
		return sensor;
	}

	void resetRunnerForNewHouse(const House& house, int currHouseDocki, int currHouseDockj);
	void setSensorForAlgorithm();

	bool isHouseCleanAndRobotInDock();
	bool isBatteryConsumedAndRobotNotInDock();
	bool getStepAndUpdateIfLegal();
	bool isRobotInDock();
	int getPositionInCompetitionForScore();


	void updateStepsRemainingOnWinner(int numStepsRemaining);
	void updateCurrHouseScoreInList(int winnerMaxSteps, int simulationSteps);

	// setters

	void setIsFinished(bool isFinished) {
		this->isFinished = isFinished;
	}

	void setAlgoRankInCompetition(int algoRankInCompetition) {
		this->algoPositionInCompetition = algoRankInCompetition;
	}

	void setFinishState(SimulationFinishState finishState) {
		this->finishState = finishState;
	}

	// getters

	bool getIsFinished() const {
		return isFinished;
	}

	std::list<int>& getHousesScore() {
		return housesScore;
	}

	int getNumSteps() const {
		return numSteps;
	}

	SimulationFinishState getFinishState() const {
		return finishState;
	}

	// static functions
	static void resetCommonDataForNewHouse(const House& house);

	static void setConfig(const map<string, int>& conf) {
		config = conf;
	}

};

#endif /* ALGORITHMRUNNER_H_ */
