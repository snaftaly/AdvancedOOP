#ifndef SIMULATOR_H_
#define SIMULATOR_H_
#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "House.h"
#include "AlgorithmRunner.h"
#include "HouseManager.h"
#include "ConfigManager.h"
#include "SimulationFinishState.h"
#include <list>
#include <map>
#include <string>

class Simulator {
	HouseManager houseMgr;
	ConfigManager confMgr;
	bool initSuccessfull;

	std::list<AbstractAlgorithm*> algorithms;
	std::list<AlgorithmRunner> algorithmRunnerList;

	// values for curr house run
	int numAlogsRunning;
	int currSuccessfullAlgoPosition;
	int numSuccessfulAlgosInRound;
	int numStepsRemaining;
	int simulationSteps;
	int winnerNumSteps;
	bool isThereAWinner;

	void updateOnSuccessfulAlgo(AlgorithmRunner& algorithmRunner);
	void fillAlgorithmList();
public:

	//C'tor signature
	Simulator(const std::string& configPath, const std::string& housesPath, const std::string& algorithmsPath);

	//D'tor signature
	~Simulator();

	void createAlgorithmRunnerList();

	//Function to run actual simulation
	void runSimulation();

	void printAlgosScores();

	void setHouseForEachAlgorithmRunner(const House& house);

	// getters
	bool isInitSuccessfull() const {
		return initSuccessfull;
	}
};

#endif /* SIMULATOR_H_ */
