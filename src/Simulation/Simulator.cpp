#include <iomanip>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unistd.h> // for usleep
#include "Simulator.h"
#include "FileUtils.h"
#include "AlgorithmRegistrar.h"


#define HOUSE_NAME_MAX 9
#define ALGO_NAME_MAX 12
#define FIRST_COLUMN_WIDTH 13
#define OTHER_COLUMN_WIDTH 10

using namespace std;


//C'tor implementation
Simulator::Simulator(const string& configPath, const string & scoreFormulaPath, const string & algorithmsPath, const string& housesPath, size_t numThreads):
	houseCounter(0), confMgr(configPath), algoMgr(algorithmsPath), houseMgr(housesPath), initSuccessfull(true)
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

	// create algo runner list
//	algoMgr.createAlgorithmRunnerList(confMgr);
}

//D'tor implementation
Simulator::~Simulator() {

}


void Simulator::threadFunc(){
	// get the next house for the thread - using mutex
	string houseFileName;
	{
		std::lock_guard<std::mutex> guard(houseReadMutex); // use mutex for this part
		if (houseCounter >= houseMgr.getHousesFileNamesLst().size()){
			return;
		}
		houseFileName = houseMgr.getHousesFileNamesLst().at(houseCounter++);
	}

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
	houseSimulation.runSimulationForHouse(this, algoMgr, currHouse, algorithmRunnerList, maxStepsAfterWinner);
}


//void Simulator::runSimulationForHouse(const House& house, list<AlgorithmRunner>& algoRunnerList){
//	int maxSteps = house.getMaxSteps(); // no need to save
//	int maxStepsAfterWinner = confMgr.getConfs().find("MaxStepsAfterWinner")->second;

//	int currSuccessfullAlgoPosition = 1;
//	int numSuccessfulAlgosInRound; // need to save
//	int winnerNumSteps = maxSteps; //need to save
//	int numStepsRemaining = maxSteps; // need to save
//	int numAlogsRunning = algoRunnerList.size(); // need to save
//	int simulationSteps = 0;
//	bool isThereAWinner = false; // need to save
//	bool isUpdatedAboutToFinish = false; // need to save
	// set the current house for the algorithm runners;
//	setHouseForEachAlgorithmRunner(house, algoRunnerList);
//
//	// run the simulation - for the different algorithms
//	while (numAlogsRunning > 0 && numStepsRemaining > 0){
//		// check if stepsRemaining == maxStepsAfterWinner
//		if (!isUpdatedAboutToFinish && simulationSteps == (maxSteps - maxStepsAfterWinner)){
//			updateAboutToFinish();
//		}
//		// reset number of successful algorithms in round (step)
//		numSuccessfulAlgosInRound = 0;
//
//		// update simulations steps and steps remaining
//		simulationSteps++;
//		numStepsRemaining--;
//		for (AlgorithmRunner& algorithmRunner : algoRunnerList){
//			if (algorithmRunner.getIsFinished()){ // if algorithm already finished - move to next one
//				continue;
//			}
//			// check if the algorithm has finished
//			if (algorithmRunner.isHouseCleanAndRobotInDock()){
//				// this part is done in case the house is already clean before making any step
//				updateOnSuccessfulAlgo(algorithmRunner, currSuccessfullAlgoPosition);
//			}
//			else if (algorithmRunner.isBatteryConsumedAndRobotNotInDock()){ // check if no more battery
//				algorithmRunner.setSimulationState(SimulationState::OutOfBattery);
//				numAlogsRunning--;
//			}
//			else {
//				bool isMadeLegalMove = algorithmRunner.getStepAndUpdateIfLegal();
//				if (!isMadeLegalMove){
//					algoMgr.addAlgoRunError(algorithmRunner.getAlgoName(), FileUtils::getFileNameNoExt(house.getFileName()), simulationSteps);
//					algorithmRunner.setSimulationState(SimulationState::IllegalMove);
//					numAlogsRunning--;
//				}
//				else { // move is legal
//					if (algorithmRunner.isHouseCleanAndRobotInDock()){
//						updateOnSuccessfulAlgo(algorithmRunner, currSuccessfullAlgoPosition);
//					}
//				}
//			}
//		}
//		// end of for loop for each algorithm - update the next successful algo position
//		currSuccessfullAlgoPosition += numSuccessfulAlgosInRound;
//
//	}
//	// end of while for the house - update algos scores
//	if (!isThereAWinner){
//		winnerNumSteps = simulationSteps;
//	}
//	for (AlgorithmRunner& algoRunner : algoRunnerList){
//		// TODO: add score to a global place
//		algoRunner.addCurrHouseScore(winnerNumSteps, simulationSteps, FileUtils::getFileNameNoExt(house.getFileName()));
//	}


//}

void Simulator::setHouseForEachAlgorithmRunner(const House& house, list<AlgorithmRunner>& algoRunnerList){
	// set common data about the house for the algorithms
	int currHouseTotDirt = house.calcDirtLevel();
	int currHouseDocki, currHouseDockj;
	std::tie(currHouseDocki, currHouseDockj) = house.getHouseDockPlace();
	for (AlgorithmRunner& algoRunner : algoRunnerList){
		algoRunner.resetRunnerForNewHouse(house, currHouseDocki, currHouseDockj, currHouseTotDirt);
	}
}


//void Simulator::updateOnSuccessfulAlgo(AlgorithmRunner& successAlgorithmRunner, int currSuccessfullAlgoPosition){
//	successAlgorithmRunner.setAlgoRankInCompetition(currSuccessfullAlgoPosition);
//	successAlgorithmRunner.setSimulationState(SimulationState::Success);
//	*numSuccessfulAlgosInRound++;
//	*numAlogsRunning--;
//	if (!(*isThereAWinner)){
//		// this algo is the first to win for the house - so update that there was a winner
//		isThereAWinner = true;
//
//		// update winner num steps
//		*winnerNumSteps =  successAlgorithmRunner.getNumSteps();
//
//		if (!(*isUpdatedAboutToFinish)){
//			// update other algos with result
//			updateAboutToFinish();
//		}
//
//	}
//}
//
//void Simulator::updateAboutToFinish(){
//
//	isUpdatedAboutToFinish = true;
//	// update steps remaining for simulator
//	numStepsRemaining = min(confMgr.getConfs().find("MaxStepsAfterWinner")->second, numStepsRemaining);
//	// update steps remaining for each algorithm
//	for (AlgorithmRunner& algorithmRunner : algoMgr.getAlgorithmRunnerList()){
//		algorithmRunner.updateStepsRemainingOnWinner(numStepsRemaining);
//	}
//}



void Simulator::runSimulation(){

	size_t numTrheads = min(numTrheads, houseMgr.getHousesFileNamesLst().size());
	vector<thread> threads;
	// create the threads
	for (int i =0 ; i< numThreads; i++){
		threads.emplace_back(threadFunc);
	}

	// loop again to join the threads
	for (auto& t : threads){
		t.join();
	}

//	for (const House& house : houseMgr.getHouses()){
//		int maxSteps = house.getMaxSteps();
//		int maxStepsAfterWinner = confMgr.getConfs().find("MaxStepsAfterWinner")->second;
//		currSuccessfullAlgoPosition = 1;
//		winnerNumSteps = numStepsRemaining = maxSteps;
//		numAlogsRunning = algoMgr.getAlgorithmRunnerList().size();
//		simulationSteps = 0;
//		isThereAWinner = false;
//		isUpdatedAboutToFinish = false;

		// set the current house for the algorithm runners;
//		setHouseForEachAlgorithmRunner(house);

		// run the simulator - for each house run the different algorithms
//		while (numAlogsRunning > 0 && numStepsRemaining > 0){
//			// check if stepsRemaining == maxStepsAfterWinner
//			if (!isUpdatedAboutToFinish && simulationSteps == (maxSteps - maxStepsAfterWinner)){
//				updateAboutToFinish();
//			}
//			// reset number of successful algorithms in round (step)
//			numSuccessfulAlgosInRound = 0;
//
//			// update simulations steps and steps remaining
//			simulationSteps++;
//			numStepsRemaining--;
//			for (AlgorithmRunner& algorithmRunner : algoMgr.getAlgorithmRunnerList()){
//				if (algorithmRunner.getIsFinished()){ // if algorithm already finished - move to next one
//					continue;
//				}
//				// check if the algorithm has finished
//				if (algorithmRunner.isHouseCleanAndRobotInDock()){
//					// this part is done in case the house is already clean before making any step
//					updateOnSuccessfulAlgo(algorithmRunner);
//				}
//				else if (algorithmRunner.isBatteryConsumedAndRobotNotInDock()){ // check if no more battery
//					algorithmRunner.setSimulationState(SimulationState::OutOfBattery);
//					numAlogsRunning--;
//				}
//				else {
//					bool isMadeLegalMove = algorithmRunner.getStepAndUpdateIfLegal();
//					if (!isMadeLegalMove){
//						algoMgr.addAlgoRunError(algorithmRunner.getAlgoName(), FileUtils::getFileNameNoExt(house.getFileName()), simulationSteps);
//						algorithmRunner.setSimulationState(SimulationState::IllegalMove);
//						numAlogsRunning--;
//					}
//					else { // move is legal
//						if (algorithmRunner.isHouseCleanAndRobotInDock()){
//							updateOnSuccessfulAlgo(algorithmRunner);
//						}
//					}
//				}
//			}
//			// end of for loop for each algorithm - update the next successful algo position
//			currSuccessfullAlgoPosition += numSuccessfulAlgosInRound;
//
//		}
//		// end of while for the house - update algos scores
//		if (!isThereAWinner){
//			winnerNumSteps = simulationSteps;
//		}
//		for (AlgorithmRunner& algoRunner : algoMgr.getAlgorithmRunnerList()){
//			algoRunner.addCurrHouseScore(winnerNumSteps, simulationSteps, FileUtils::getFileNameNoExt(house.getFileName()));
//		}
//	}

}



void Simulator::printRowSeparator(const int tableWidth){
	cout << setw(tableWidth) << setfill('-') << "" << endl << setfill(' ');;
}

void Simulator::printTableHeader(const int tableWidth){
	//print row separator
	printRowSeparator(tableWidth);
	// print houses names
	cout << "|             |";
	for (const House& house : houseMgr.getHouses()){
		string name =  FileUtils::getFileNameNoExt(house.getFileName());
		name.resize(HOUSE_NAME_MAX, ' ');
		cout << name << " |";
	}
	cout << std::left << std::setw(10) << "AVG" << "|" << endl;
	//print row separator
	printRowSeparator(tableWidth);
}


void Simulator::printResults(){
	// check if all houses were problematic
	if (houseMgr.getHousesErrors().size() == houseMgr.getHousesFileNamesLst().size()){
		// all the houses were problematic
		houseMgr.printHousesErrors(true);
		return;
	}

	int numHouses = houseMgr.getHousesFileNamesLst().size();
	int tableWidth = FIRST_COLUMN_WIDTH + 2 + (OTHER_COLUMN_WIDTH + 1)*(numHouses+1);
	int scoreAlgoHouse;
	int scoreSumForAlgo;
	printTableHeader(tableWidth);

	for (AlgorithmRunner& algoRunner : algoMgr.getAlgorithmRunnerList()){
		string algoNameTrimmed = algoRunner.getAlgoName();
		algoNameTrimmed.resize(ALGO_NAME_MAX, ' ');
		cout << "|" << algoNameTrimmed << " |";
		scoreSumForAlgo = 0;
		// print each house score
		for (const House& house : houseMgr.getHouses()){
			scoreAlgoHouse = algoRunner.getHousesScore().find(FileUtils::getFileNameNoExt(house.getFileName()))->second;
			scoreSumForAlgo += scoreAlgoHouse;
			cout <<  right <<  setw(10) << scoreAlgoHouse << "|";
		}
		// print average
		float avgForAlgo = (float) scoreSumForAlgo/numHouses;
		//print row separator
		cout << fixed << setprecision(2) << right<< setw(10) << avgForAlgo  << "|" << endl;
		printRowSeparator(tableWidth);
	}
}

void Simulator::printErrors(){
	if (houseMgr.getHousesErrors().empty() && algoMgr.getAlgorithmsLoadErrors().empty() && algoMgr.getAlgorithmsRunErrors().empty()){
		return;
	}
	cout << endl << "Errors:" << endl;
	houseMgr.printHousesErrors(false);
	algoMgr.printAlgorithmsErrors(false);
}