#include <iomanip>
#include <iostream>
#include <sstream>
#include "Simulator.h"
#include "SimpleAlgorithm.h"
#include "FileUtils.h"
using namespace std;

//C'tor implementation
Simulator::Simulator(const string& configPath, const string& housesPath, const string& algorithmsPath):
	houseMgr(housesPath), confMgr(configPath), initSuccessfull(true)
{

	// get configurations from file
	if (!confMgr.loadFromFile()){
		initSuccessfull = false;
		return;
	}

	// check if there are valid houses
	if (!houseMgr.readHousesFiles()){
		initSuccessfull = false;
		return;
	}

	// insert simple algorithm * to algorithms:
	fillAlgorithmList();

	AlgorithmRunner::setConfig(confMgr.getConfs());
	createAlgorithmRunnerList();
}

//D'tor implementation
Simulator::~Simulator() {
	// delete from memory all the dynamically created algorithms
	for (AbstractAlgorithm* algo : algorithms){
		delete algo;
	}
}


void Simulator::createAlgorithmRunnerList(){
	for (AbstractAlgorithm* algo : algorithms){
		// set the configurations for the algorithm
		algo->setConfiguration(confMgr.getConfs());

		// put the algorithm in the algoRunner list
		algorithmRunnerList.emplace_back(algo); // TODO use emplace the right way
	}
	for (AlgorithmRunner& algorithmRunner : algorithmRunnerList){
		algorithmRunner.setSensorForAlgorithm(); // TODO: move this to algorunner init (line above)
	}
}


void Simulator::runSimulation(){
	for (const House& house : houseMgr.getHouses()){

		int maxSteps = house.getMaxSteps();
		currSuccessfullAlgoPosition = 1;
		winnerNumSteps = numStepsRemaining = maxSteps;
		numAlogsRunning = algorithmRunnerList.size();
		simulationSteps = 0;
		isThereAWinner = false;

		// set the current house for the algorithm runners;
		setHouseForEachAlgorithmRunner(house);

		// run the simulator - for each house run the different algorithms
		while (numAlogsRunning > 0 && numStepsRemaining > 0){
			numSuccessfulAlgosInRound = 0;
			for (AlgorithmRunner& algorithmRunner : algorithmRunnerList){
				if (algorithmRunner.getIsFinished()){
					continue;
				}
				// check if the algorithm has finished
				if (algorithmRunner.isHouseCleanAndRobotInDock()){
					// this part is done in case the house is already clean before making any step
					updateOnSuccessfulAlgo(algorithmRunner);
				}
				else if (algorithmRunner.isBatteryConsumedAndRobotNotInDock()){ // check if no more battery
					algorithmRunner.setIsFinished(true);
					algorithmRunner.setFinishState(SimulationFinishState::OutOfBattery);
					numAlogsRunning--;
				}
				else {
					bool isMadeLegalMove = algorithmRunner.getStepAndUpdateIfLegal();
					if (!isMadeLegalMove){
						cout << "Error: algorithm made an illegal step." << endl;
						algorithmRunner.setIsFinished(true);
						algorithmRunner.setFinishState(SimulationFinishState::IllegalMove);
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
			simulationSteps++;
			numStepsRemaining--;
			currSuccessfullAlgoPosition += numSuccessfulAlgosInRound;
		}
		// end of while for the house - update algos scores
		for (AlgorithmRunner& algoRunner : algorithmRunnerList){
			if (!isThereAWinner){
				winnerNumSteps = simulationSteps;
			}
			algoRunner.updateCurrHouseScoreInList(winnerNumSteps, simulationSteps);
		}
	}

}

void Simulator::setHouseForEachAlgorithmRunner(const House& house){
	// set common data about the house for the algorithms
	AlgorithmRunner::resetCommonDataForNewHouse(house);
	int currHouseDocki, currHouseDockj;
	std::tie(currHouseDocki, currHouseDockj) = house.getHouseDockPlace();
	for (AlgorithmRunner& algoRunner : algorithmRunnerList){
		algoRunner.resetRunnerForNewHouse(house, currHouseDocki, currHouseDockj);
	}
}

void Simulator::updateOnSuccessfulAlgo(AlgorithmRunner& successAlgorithmRunner){
	successAlgorithmRunner.setIsFinished(true);
	successAlgorithmRunner.setAlgoRankInCompetition(currSuccessfullAlgoPosition);
	successAlgorithmRunner.setFinishState(SimulationFinishState::Success);
	numSuccessfulAlgosInRound++;
	numAlogsRunning--;
	if (!isThereAWinner && currSuccessfullAlgoPosition == 1){ // this algo is the first to win for the house
		isThereAWinner = true;

		// update winner num steps
		winnerNumSteps =  successAlgorithmRunner.getNumSteps();

		// update steps to run to the minimum of MaxStepsAfterWinner and numStepsRemaining
		 numStepsRemaining = min(confMgr.getConfs().find("MaxStepsAfterWinner")->second, numStepsRemaining);

		// update other algos with result
		for (AlgorithmRunner& algorithmRunner : algorithmRunnerList){
			algorithmRunner.updateStepsRemainingOnWinner(numStepsRemaining);
		}

	}
}

void Simulator::fillAlgorithmList(){
	algorithms.push_back(new SimpleAlgorithm()); // TODO: change this to push_back!
}

void Simulator::printTableHeader(const string & rowSeparator){
	//print row separator
		cout << rowSeparator << endl;
		// print houses names
		cout << "|             |";
		for (const House& house : houseMgr.getHouses()){
			string name =  FileUtils::getFileNameNoExt(house.getFileName());
			name.resize(9, ' ');
			cout << name << " |";
		}
		cout << "AVG       |" << endl;
		//print row separator
		cout << rowSeparator << endl;
}


void Simulator::printAlgosScores(){
	int numHouses = houseMgr.getHouses().size();
	int tableWidth = 15 + 11*(numHouses+1); //TODO: use macros

	// create the row separator
	string rowSeparator = "";
	rowSeparator.insert(0, tableWidth, '-');

	printTableHeader(rowSeparator);

	for (AlgorithmRunner& algoRunner : algorithmRunnerList){
		cout << "|" << algoRunner.getAlgoName() << " |";
		int scoreSumForAlgo = 0;
		for (list<int>::iterator housesScoreitr = algoRunner.getHousesScore().begin();
				housesScoreitr != algoRunner.getHousesScore().end(); housesScoreitr++){
			scoreSumForAlgo += *housesScoreitr;
			string scoreStr = std::to_string(*housesScoreitr);
			string scoreSpace = "";
			scoreSpace.insert(0, 10 - scoreStr.size(), ' ');
			cout << scoreSpace << scoreStr << "|";
		}
		float avgForAlgo = (float) scoreSumForAlgo/numHouses;
		stringstream stream;
		stream << fixed << setprecision(2) << avgForAlgo;
		string avgStr = stream.str();
		string avgSpace = "";
		avgSpace.insert(0, 10 - avgStr.size(), ' ');
		cout << avgSpace << avgStr << "|" << endl;
		//print row separator
		cout << rowSeparator << endl;
	}
}

void Simulator::printErrors(){
	if (houseMgr.getHousesErrors().empty()){ // TODO: add here handling of algorithm errors (with &&
		return;
	}
	cout << "Errors:" << endl;
	houseMgr.printHousesErrors(false);
	// TODO: print algos errors
}
