#include <iomanip>
#include <iostream>
#include <sstream>
#include "Simulator.h"
#include "FileUtils.h"

#define HOUSE_NAME_MAX 9
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

	//TODO: remove this
//	confMgr.printConfs();
//	initSuccessfull = false;
//	return;

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

	// insert simple algorithm * to algorithms:

	// put confs in algorithm runner
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
			numSuccessfulAlgosInRound = 0;
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
						cout << "Error: algorithm made an illegal step." << endl;
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
			// end of for loop for each algorithm - update step remaining
			// TODO: should we move it to be before going over each algo?
			simulationSteps++;
			numStepsRemaining--;
			currSuccessfullAlgoPosition += numSuccessfulAlgosInRound;
		}
		// end of while for the house - update algos scores
		if (!isThereAWinner){
			winnerNumSteps = simulationSteps;
		}
		for (AlgorithmRunner& algoRunner : algoMgr.getAlgorithmRunnerList()){
			algoRunner.updateCurrHouseScoreInList(winnerNumSteps, simulationSteps);
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
	if (!isThereAWinner){ // && currSuccessfullAlgoPosition == 1){ //TODO: do we need the second check?
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

//void Simulator::fillAlgorithmList(){ // TODO: this is not needed
//	algorithms.push_back(new SimpleAlgorithm());
//}

void Simulator::printTableHeader(const string & rowSeparator){
	//print row separator
		cout << rowSeparator << endl;
		// print houses names
		cout << "|             |";
		for (const House& house : houseMgr.getHouses()){
			string name =  FileUtils::getFileNameNoExt(house.getFileName());
			name.resize(HOUSE_NAME_MAX, ' ');
			cout << name << " |";
		}
		cout << "AVG       |" << endl;
		//print row separator
		cout << rowSeparator << endl;
}


void Simulator::printAlgosScores(){
	int numHouses = houseMgr.getHouses().size();
	int tableWidth = FIRST_COLUMN_WIDTH + 2 + (OTHER_COLUMN_WIDTH + 1)*(numHouses+1);

	// create the row separator
	string rowSeparator = "";
	rowSeparator.insert(0, tableWidth, '-');

	printTableHeader(rowSeparator);

	for (AlgorithmRunner& algoRunner : algoMgr.getAlgorithmRunnerList()){
		cout << "|" << algoRunner.getAlgoName() << " |";
		int scoreSumForAlgo = 0;
		// print each house score // TODO: we assume here that the houses score is a list and not a map
		for (list<int>::iterator housesScoreitr = algoRunner.getHousesScore().begin();
				housesScoreitr != algoRunner.getHousesScore().end(); housesScoreitr++){
			scoreSumForAlgo += *housesScoreitr;
			string scoreStr = std::to_string(*housesScoreitr);
			string scoreSpace = "";
			scoreSpace.insert(0, OTHER_COLUMN_WIDTH - scoreStr.size(), ' ');
			cout << scoreSpace << scoreStr << "|";
		}
		// print average
		float avgForAlgo = (float) scoreSumForAlgo/numHouses;
		stringstream stream;
		stream << fixed << setprecision(2) << avgForAlgo;
		string avgStr = stream.str();
		string avgSpace = "";
		avgSpace.insert(0, OTHER_COLUMN_WIDTH - avgStr.size(), ' ');
		cout << avgSpace << avgStr << "|" << endl;
		//print row separator
		cout << rowSeparator << endl;
	}
}

void Simulator::printErrors(){
	if (houseMgr.getHousesErrors().empty() && algoMgr.getAlgorithmsErrors().empty()){
		return;
	}
	cout << "Errors:" << endl;
	houseMgr.printHousesErrors(false);
	algoMgr.printAlgorithmsErrors(false);
}
