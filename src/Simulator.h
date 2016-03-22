#ifndef SIMULATOR_H_
#define SIMULATOR_H_
#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "House.h"
#include "AlgorithmRunner.h"
#include "HouseManager.h"
#include "ConfigManager.h"
#include <list>
#include <map>
#include <string>

class Simulator {
	HouseManager houseMgr;
	ConfigManager confMgr;

	std::list<AbstractAlgorithm*> algorithms;
	std::list<AlgorithmRunner> algorithmRunnerList;

	// values for curr house run
	int numAlogsRunning;
	int currSuccessfullAlgoPosition;
	int numSuccessfulAlgosInRound;
	int numStepsRemaining;
	int winnerNumSteps;

	void updateOnSuccessfulAlgo(AlgorithmRunner& algorithmRunner);
public:

	//C'tor signature
	Simulator(const std::string& iniPath, const std::string& housesPath);

	//D'tor signature
	~Simulator();

	void createAlgorithmRunnerList();

	//Function to run actual simulation
	void runSimulation();

	void printAlgosScores();

	void setHouseForEachAlgorithmRunner(const House& house) const;


};

#endif /* SIMULATOR_H_ */
