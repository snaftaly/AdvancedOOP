#ifndef SIMULATOR_H_
#define SIMULATOR_H_

#include <thread>
#include <atomic>
#include <mutex>
#include <list>
#include <map>
#include <string>
#include "House.h"
#include "AlgorithmRunner.h"
#include "AlgorithmManager.h"
#include "HouseManager.h"
#include "ConfigManager.h"
#include "SimulationState.h"
#include "HouseSimulation.h"
#include "ScoreManager.h"



class Simulator {

	std::atomic<size_t> houseIndex;

	ConfigManager confMgr;
	ScoreManager scoreMgr;
	AlgorithmManager algoMgr;
	HouseManager houseMgr;
	VideoManager videoMgr;
	size_t numThreads;
	bool initSuccessfull;

	std::map<std::string, std::map<std::string, int>> results;

	void updateOnSuccessfulAlgo(AlgorithmRunner& algorithmRunner);
	void updateAboutToFinish();
	void runSingleSubSimulationThread(bool doVideo);
	void runSingleSubSimulation(const string& houseFileName, bool doVideo);
public:

	//C'tor signature
	Simulator(const string& configPath, const string & scoreFormulaPath, const string & algorithmsPath, const string& housesPath, size_t numThreads);

	//D'tor signature
	~Simulator();

	void createAlgorithmRunnerList();

	//Function to run actual simulation
	void runSimulation(bool doVideo);

	void printResults();
	void printTableHeader(const int tableWidth);
	void printRowSeparator(const int tableWidth);

	void printErrors();

	void setHouseForEachAlgorithmRunner(const House& house,  list<AlgorithmRunner>& algoRunnerList);

	// getters
	bool isInitSuccessfull() const {
		return initSuccessfull;
	}
};

#endif /* SIMULATOR_H_ */
