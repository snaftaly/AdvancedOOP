#include <iomanip>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unistd.h> // for usleep
#include "Simulator.h"
#include "FileUtils.h"
#include "AlgorithmRegistrar.h"
#include "../Common/MakeUniqueAdder.h"


#define HOUSE_NAME_MAX 9
#define ALGO_NAME_MAX 12
#define FIRST_COLUMN_WIDTH 13
#define OTHER_COLUMN_WIDTH 10

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

	// score formula init TODO: add this part

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

	// put confs in algorithm runner staticaly
	AlgorithmRunner::setConfig(confMgr.getConfs());

}

//D'tor implementation
Simulator::~Simulator() {

}


void Simulator::runSingleSubSimulationThread(){
	// fetch old value, then add.
	for (size_t index = houseIndex++; index < houseMgr.getHousesFileNamesLst().size();  index = houseIndex++){
		runSingleSubSimulation(houseMgr.getHousesFileNamesLst().at(index));
		cout << "running for house number: " << index << endl;
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

	size_t numNeededTrheads = min(numThreads, houseMgr.getHousesFileNamesLst().size());
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



void Simulator::printRowSeparator(const int tableWidth){
	cout << setw(tableWidth) << setfill('-') << "" << endl << setfill(' ');;
}

void Simulator::printTableHeader(const int tableWidth){
	//print row separator
	printRowSeparator(tableWidth);
	// print houses names
	cout << "|             |";
	for (const string& houseFileName : houseMgr.getHousesFileNamesLst()){
		if (houseMgr.getHousesErrors().find(houseFileName) != houseMgr.getHousesErrors().end()){
			// house had errors so continue to next
			continue;
		}
		string name =  FileUtils::getFileNameNoExt(houseFileName);
		name.resize(HOUSE_NAME_MAX, ' ');
		cout << name << " |";
	}
	cout << std::left << std::setw(10) << "AVG" << "|" << endl;
	//print row separator
	printRowSeparator(tableWidth);
}


void Simulator::printResults(){
	// TODO: sort by AVG
	// check if all houses were problematic
	int numValidHouses = houseMgr.getHousesFileNamesLst().size() - houseMgr.getHousesErrors().size();

	if (numValidHouses == 0){
		// all the houses were problematic
		houseMgr.printHousesErrors(true);
		return;
	}

	int tableWidth = FIRST_COLUMN_WIDTH + 2 + (OTHER_COLUMN_WIDTH + 1)*(numValidHouses+1);
	int scoreAlgoHouse;
	int scoreSumForAlgo;
	printTableHeader(tableWidth);

	for (const auto& algoNameHouseScorePair : scoreMgr.getAlgosScoresForHouses()){
		string algoNameTrimmed = algoNameHouseScorePair.first;
		algoNameTrimmed.resize(ALGO_NAME_MAX, ' ');
		cout << "|" << algoNameTrimmed << " |";
		scoreSumForAlgo = 0;
		// print each house score
		for (const string& houseFileName : houseMgr.getHousesFileNamesLst()){
			if (houseMgr.getHousesErrors().find(houseFileName) != houseMgr.getHousesErrors().end()){
				// house had errors so continue to next
				continue;
			}
			scoreAlgoHouse = algoNameHouseScorePair.second.find(FileUtils::getFileNameNoExt(houseFileName))->second;
			scoreSumForAlgo += scoreAlgoHouse;
			cout <<  right <<  setw(10) << scoreAlgoHouse << "|";
		}
		// print average
		float avgForAlgo = (float) scoreSumForAlgo/numValidHouses;
		//print row separator
		cout << fixed << setprecision(2) << right<< setw(10) << avgForAlgo  << "|" << endl;
		printRowSeparator(tableWidth);
	}

	printErrors();
}

void Simulator::printErrors(){
	if (houseMgr.getHousesErrors().empty() && algoMgr.getAlgorithmsLoadErrors().empty() && algoMgr.getAlgorithmsRunErrors().empty()){
		return;
	}
	cout << endl << "Errors:" << endl;
	houseMgr.printHousesErrors(false);
	algoMgr.printAlgorithmsErrors(false);
}
