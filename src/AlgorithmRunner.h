#ifndef ALGORITHMRUNNER_H_
#define ALGORITHMRUNNER_H_
#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "Sensor.h"
#include "SimulationState.h"
#include <list>
#include <tuple>
#include <map>
#include <string>

class AlgorithmRunner {
	AbstractAlgorithm* algorithm;
	Sensor sensor;
	std::string algoName;

	// current house info:
	House currHouse;
	int roboti, robotj;

	int batteryLevel;
	int numSteps;
	int dirtCollected;

	int algoPositionInCompetition;
	SimulationState simulationState;

	std::map<std::string, int> housesScores;

	static map<string, int> config;

	static int currHouseTotDirt;

	bool isLegalStep(int stepi, int stepj);

	void printSimulation(int stepi, int stepj);
public:

	AlgorithmRunner(AbstractAlgorithm* a, string algoName);

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
	void addCurrHouseScore(const int winnerNumSteps, const int simulationSteps, const string& houseName);

	// setters


	void setAlgoRankInCompetition(int algoRankInCompetition) {
		this->algoPositionInCompetition = algoRankInCompetition;
	}

	void setSimulationState(SimulationState SimState) {
		this->simulationState = SimState;
	}

	// getters

	bool getIsFinished() const {
		return (this->simulationState != SimulationState::Running);
	}

	std::map<std::string, int>& getHousesScore() {
		return housesScores;
	}

	int getNumSteps() const {
		return numSteps;
	}

	SimulationState getFinishState() const {
		return simulationState;
	}

	const std::string& getAlgoName() const {
		return algoName;
	}

	const AbstractAlgorithm* getAlgorithm() const {
		return algorithm;
	}

	// static functions
	static void resetCommonDataForNewHouse(const House& house);

	static void setConfig(const map<string, int>& conf) {
		config = conf;
	}
};

#endif /* ALGORITHMRUNNER_H_ */
