#include <iomanip>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <unistd.h> // for usleep
#include "Simulator.h"
#include "FileUtils.h"

#define HOUSE_NAME_MAX 9
#define ALGO_NAME_MAX 12
#define FIRST_COLUMN_WIDTH 13
#define OTHER_COLUMN_WIDTH 10

using namespace std;

//C'tor implementation
Simulator::Simulator(const string& configPath, const string & algorithmsPath, const string& housesPath):
	confMgr(configPath), algoMgr(algorithmsPath), houseMgr(housesPath), initSuccessfull(true)
{

	// get configurations from file
	if (!confMgr.loadFromFile()){
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

	// put confs in algorithm runner staticaly
	AlgorithmRunner::setConfig(confMgr.getConfs());

	// create algo runner list
	algoMgr.createAlgorithmRunnerList(confMgr);
}

//D'tor implementation
Simulator::~Simulator() {

}

void Simulator::runSimulation(){
	for (const House& house : houseMgr.getHouses()){
		int maxSteps = house.getMaxSteps();
		int maxStepsAfterWinner = confMgr.getConfs().find("MaxStepsAfterWinner")->second;
		currSuccessfullAlgoPosition = 1;
		winnerNumSteps = numStepsRemaining = maxSteps;
		numAlogsRunning = algoMgr.getAlgorithmRunnerList().size();
		simulationSteps = 0;
		isThereAWinner = false;
		isUpdatedAboutToFinish = false;

		// set the current house for the algorithm runners;
		setHouseForEachAlgorithmRunner(house);

		// run the simulator - for each house run the different algorithms
		while (numAlogsRunning > 0 && numStepsRemaining > 0){
			// check if stepsRemaining == maxStepsAfterWinner
			if (!isUpdatedAboutToFinish && simulationSteps == (maxSteps - maxStepsAfterWinner)){
				updateAboutToFinish();
			}
			// reset number of successful algorithms in round (step)
			numSuccessfulAlgosInRound = 0;

			// update simulations steps and steps remaining
			simulationSteps++;
			numStepsRemaining--;
			for (AlgorithmRunner& algorithmRunner : algoMgr.getAlgorithmRunnerList()){
				if (algorithmRunner.getIsFinished()){ // if algorithm already finished - move to next one
					continue;
				}
				// check if the algorithm has finished
				if (algorithmRunner.isHouseCleanAndRobotInDock()){
					// this part is done in case the house is already clean before making any step
					updateOnSuccessfulAlgo(algorithmRunner);
				}
				else if (algorithmRunner.isBatteryConsumedAndRobotNotInDock()){ // check if no more battery
					algorithmRunner.setSimulationState(SimulationState::OutOfBattery);
					numAlogsRunning--;
				}
				else {
					bool isMadeLegalMove = algorithmRunner.getStepAndUpdateIfLegal();
					if (!isMadeLegalMove){
						algoMgr.addAlgoRunError(algorithmRunner.getAlgoName(), FileUtils::getFileNameNoExt(house.getFileName()), simulationSteps);
						algorithmRunner.setSimulationState(SimulationState::IllegalMove);
						numAlogsRunning--;
					}
					else { // move is legal
						if (algorithmRunner.isHouseCleanAndRobotInDock()){
							updateOnSuccessfulAlgo(algorithmRunner);
						}
					}
				}
			}
			// end of for loop for each algorithm - update the next successful algo position
			currSuccessfullAlgoPosition += numSuccessfulAlgosInRound;

		}
		// end of while for the house - update algos scores
		if (!isThereAWinner){
			winnerNumSteps = simulationSteps;
		}
		for (AlgorithmRunner& algoRunner : algoMgr.getAlgorithmRunnerList()){
			algoRunner.addCurrHouseScore(winnerNumSteps, simulationSteps, FileUtils::getFileNameNoExt(house.getFileName()));
		}
	}

}

void Simulator::setHouseForEachAlgorithmRunner(const House& house){
	// set common data about the house for the algorithms
	AlgorithmRunner::resetCommonDataForNewHouse(house);
	int currHouseDocki, currHouseDockj;
	std::tie(currHouseDocki, currHouseDockj) = house.getHouseDockPlace();
	for (AlgorithmRunner& algoRunner : algoMgr.getAlgorithmRunnerList()){
		algoRunner.resetRunnerForNewHouse(house, currHouseDocki, currHouseDockj);
	}
}


void Simulator::updateOnSuccessfulAlgo(AlgorithmRunner& successAlgorithmRunner){
	successAlgorithmRunner.setAlgoRankInCompetition(currSuccessfullAlgoPosition);
	successAlgorithmRunner.setSimulationState(SimulationState::Success);
	numSuccessfulAlgosInRound++;
	numAlogsRunning--;
	if (!isThereAWinner){
		// this algo is the first to win for the house - so update that there was a winner
		isThereAWinner = true;

		// update winner num steps
		winnerNumSteps =  successAlgorithmRunner.getNumSteps();

		if (!isUpdatedAboutToFinish){
			// update other algos with result
			updateAboutToFinish();
		}

	}
}

void Simulator::updateAboutToFinish(){

	isUpdatedAboutToFinish = true;
	// update steps remaining for simulator
	numStepsRemaining = min(confMgr.getConfs().find("MaxStepsAfterWinner")->second, numStepsRemaining);
	// update steps remaining for each algorithm
	for (AlgorithmRunner& algorithmRunner : algoMgr.getAlgorithmRunnerList()){
		algorithmRunner.updateStepsRemainingOnWinner(numStepsRemaining);
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
	for (const House& house : houseMgr.getHouses()){
		string name =  FileUtils::getFileNameNoExt(house.getFileName());
		name.resize(HOUSE_NAME_MAX, ' ');
		cout << name << " |";
	}
	cout << std::left << std::setw(10) << "AVG" << "|" << endl;
	//print row separator
	printRowSeparator(tableWidth);
}


void Simulator::printAlgosScores(){
	int numHouses = houseMgr.getHouses().size();
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
