#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <list>
#include <map>
#include <string>
#include "House.h"
#include "AlgorithmRunner.h"
#include "AlgorithmManager.h"
#include "HouseManager.h"
#include "ConfigManager.h"
#include "SimulationState.h"


class Simulator {
	ConfigManager confMgr;
	AlgorithmManager algoMgr;
	HouseManager houseMgr;
	bool initSuccessfull;

	// values for curr house run
	int numStepsRemaining;
	int simulationSteps;
	int numAlogsRunning;
	int currSuccessfullAlgoPosition;
	int numSuccessfulAlgosInRound;
	int winnerNumSteps;
	bool isThereAWinner;
	bool isUpdatedAboutToFinish;

	void updateOnSuccessfulAlgo(AlgorithmRunner& algorithmRunner);
	void updateAboutToFinish();
public:

	//C'tor signature
	Simulator::Simulator(const string& configPath, const string & scoreFormulaPath, const string & algorithmsPath, const string& housesPath, int numThreads);

	//D'tor signature
	~Simulator();

	void createAlgorithmRunnerList();

	//Function to run actual simulation
	void runSimulation();

	void printAlgosScores();
	void printTableHeader(const int tableWidth);
	void printRowSeparator(const int tableWidth);

	void printErrors();

	void setHouseForEachAlgorithmRunner(const House& house);

	// getters
	bool isInitSuccessfull() const {
		return initSuccessfull;
	}
};

#endif /* SIMULATOR_H_ */
