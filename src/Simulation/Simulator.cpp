#include <iomanip>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <unistd.h> // for usleep
#include "Simulator.h"
#include "FileUtils.h"
#include "AlgorithmRegistrar.h"
#include "../Common/MakeUniqueAdder.h"

using namespace std;


//C'tor implementation
Simulator::Simulator(const string& configPath, const string & scoreFormulaPath, const string & algorithmsPath, const string& housesPath, size_t _numThreads):
	houseIndex(0), confMgr(configPath), scoreMgr(scoreFormulaPath), algoMgr(algorithmsPath), houseMgr(housesPath), numThreads(_numThreads), initSuccessfull(true)
{

	// get configurations from file
	if (!confMgr.loadFromFile()){
		initSuccessfull = false;
		return;
	}

	// score formula init
	if (!scoreMgr.loadFormula()){
		initSuccessfull = false;
		return;
	}

	// get algorithms
	if (!algoMgr.readAlgoFiles()){
		initSuccessfull = false;
		return;
	}

	// get houses
	if (!houseMgr.readHousesFiles()){
		initSuccessfull = false;
		return;
	}

	// prepare the scores list
	AlgorithmRegistrar& registrar = AlgorithmRegistrar::getInstance();
	auto& algorithmNames = registrar.getAlgorithmNames();
	scoreMgr.prepareScoresList(algorithmNames);

	// put confs in algorithm runner staticaly
	AlgorithmRunner::setConfig(confMgr.getConfs());

}

//D'tor implementation
Simulator::~Simulator() {

}


void Simulator::runSingleSubSimulationThread(){
	// fetch old value, then add.
	for (size_t index = houseIndex++; index < houseMgr.getHousesFileNames().size();  index = houseIndex++){
		runSingleSubSimulation(houseMgr.getHousesFileNames().at(index));
	}
}


void Simulator::runSingleSubSimulation(const string& houseFileName){
	House currHouse;
	if (!(currHouse.readFromFile(houseMgr.getHousesPath() + houseFileName))){
		// there was an error reading from house from file, continue to next file
		houseMgr.addHouseErr(houseFileName, currHouse.getErrStr());
		return;
	}
	currHouse.fixHouse(); // fix the number of columns+rows and walls
	if (!currHouse.isHouseValid()){
		houseMgr.addHouseErr(houseFileName, currHouse.getErrStr());
		return;
	}
	// house is valid - add to valid names.
	houseMgr.addValidHouse(houseFileName);

	// create the algo runner list
	AlgorithmRegistrar& registrar = AlgorithmRegistrar::getInstance();
	list<AlgorithmRunner> algorithmRunnerList;

	auto algorithms = registrar.getAlgorithms();
	auto& algorithmNames = registrar.getAlgorithmNames();
	auto pName = algorithmNames.begin();
	for(auto& algo: algorithms) {
		algo->setConfiguration(confMgr.getConfs());
		algorithmRunnerList.emplace_back(algo, *(pName++));
	}

	// set the current house for the algorithm runners;
	setHouseForEachAlgorithmRunner(currHouse, algorithmRunnerList);

	// create an instance to run the simulation for the house
	HouseSimulation houseSimulation;
	size_t maxStepsAfterWinner = confMgr.getConfs().find("MaxStepsAfterWinner")->second;
	houseSimulation.runSimulationForHouse(algoMgr, scoreMgr, currHouse, algorithmRunnerList, maxStepsAfterWinner);
}

void Simulator::setHouseForEachAlgorithmRunner(const House& house, list<AlgorithmRunner>& algoRunnerList){
	// set common data about the house for the algorithms
	int currHouseTotDirt = house.calcDirtLevel();
	int currHouseDocki, currHouseDockj;
	std::tie(currHouseDocki, currHouseDockj) = house.getHouseDockPlace();
	for (AlgorithmRunner& algoRunner : algoRunnerList){
		algoRunner.resetRunnerForNewHouse(house, currHouseDocki, currHouseDockj, currHouseTotDirt);
	}
}

void Simulator::runSimulation(){

	size_t numNeededTrheads = min(numThreads, houseMgr.getHousesFileNames().size());
	vector<unique_ptr<thread>> threads(numNeededTrheads);	// create the threads

    for(auto& thread_ptr : threads) {
        // ===> actually create the threads and run them
        thread_ptr = make_unique<thread>(&Simulator::runSingleSubSimulationThread, this); // create and run the thread
    }

    // ===> join all the threads to finish nicely (i.e. without crashing / terminating threads)
    for(auto& thread_ptr : threads) {
        thread_ptr->join();
    }
}



void Simulator::printResults(){
	// check if all houses were problematic
	if ((houseMgr.getValidHousesFileNamesSorted().size()) == 0){
		// all the houses were problematic
		houseMgr.printHousesErrors(true);
		return;
	}

	// print score table sorted by avg
	scoreMgr.printScoreTable(houseMgr.getValidHousesFileNamesSorted());

	// print all the errors
	printErrors();
}

void Simulator::printErrors(){
	if (houseMgr.getHousesErrors().empty() && algoMgr.getAlgorithmsLoadErrors().empty() && algoMgr.getAlgorithmsRunErrors().empty()){
		return;
	}
	cout << endl << "Errors:" << endl;
	houseMgr.printHousesErrors(false);
	algoMgr.printAlgorithmsErrors(false);
	scoreMgr.printError();
}
