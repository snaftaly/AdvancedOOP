#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include "House.h"
#include "AlgorithmRunner.h"
#include "AlgorithmManager.h"
#include "HouseManager.h"
#include "ConfigManager.h"
#include "SimulationState.h"
#include <list>
#include <map>
#include <string>

class Simulator {
	ConfigManager confMgr;
	AlgorithmManager algoMgr;
	HouseManager houseMgr;
	bool initSuccessfull;

	// values for curr house run
	int numAlogsRunning;
	int currSuccessfullAlgoPosition;
	int numSuccessfulAlgosInRound;
	int numStepsRemaining;
	int simulationSteps;
	int winnerNumSteps;
	bool isThereAWinner;
	bool isUpdatedAboutToFinish;

	void updateOnSuccessfulAlgo(AlgorithmRunner& algorithmRunner);
	void updateAboutToFinish();
	void fillAlgorithmList();
public:

	//C'tor signature
	Simulator(const std::string& configPath,  const std::string& algorithmsPath, const std::string& housesPath);

	//D'tor signature
	~Simulator();

	void createAlgorithmRunnerList();

	//Function to run actual simulation
	void runSimulation();

	void printAlgosScores();
	void printTableHeader(const string & rowseparator);

	void printErrors();

	void setHouseForEachAlgorithmRunner(const House& house);

	// getters
	bool isInitSuccessfull() const {
		return initSuccessfull;
	}
};

#endif /* SIMULATOR_H_ */
