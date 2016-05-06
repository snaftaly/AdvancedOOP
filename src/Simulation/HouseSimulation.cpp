/*
 * HouseSimulation.cpp
 *
 *  Created on: May 5, 2016
 *      Author: snaftaly
 */

#include "HouseSimulation.h"
#include "FileUtils.h"

using namespace std;

HouseSimulation::HouseSimulation() {
	// TODO Auto-generated constructor stub

}

HouseSimulation::~HouseSimulation() {
	// TODO Auto-generated destructor stub
}

void HouseSimulation::runSimulationForHouse(AlgorithmManager& algoMgr, ScoreManager& scoreMgr, const House& house,
		list<AlgorithmRunner>& algoRunnerList, const int _maxStepsAfterWinner){
	int maxSteps = house.getMaxSteps(); // no need to save

	currSuccessfullAlgoPosition = 1;
	winnerNumSteps = maxSteps;
	numStepsRemaining = maxSteps;
	numAlogsRunning = algoRunnerList.size();
	simulationSteps = 0;
	isThereAWinner = false;
	isUpdatedAboutToFinish = false;
	maxStepsAfterWinner = _maxStepsAfterWinner;

	// run the simulation - for the different algorithms
	while (numAlogsRunning > 0 && numStepsRemaining > 0){
		// check if stepsRemaining == maxStepsAfterWinner
		if (!isUpdatedAboutToFinish && simulationSteps == (maxSteps - maxStepsAfterWinner)){
			updateAboutToFinish(algoRunnerList);
		}
		// reset number of successful algorithms in round (step)
		numSuccessfulAlgosInRound = 0;

		// update simulations steps and steps remaining
		simulationSteps++;
		numStepsRemaining--;
		for (AlgorithmRunner& algorithmRunner : algoRunnerList){
			if (algorithmRunner.getIsFinished()){ // if algorithm already finished - move to next one
				continue;
			}
			// check if the algorithm has finished
			if (algorithmRunner.isHouseCleanAndRobotInDock()){
				// this part is done in case the house is already clean before making any step
				updateOnSuccessfulAlgo(algorithmRunner, algoRunnerList);
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
						updateOnSuccessfulAlgo(algorithmRunner, algoRunnerList);
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
	int score;
	for (AlgorithmRunner& algoRunner : algoRunnerList){
		score = algoRunner.getCurrHouseScore(winnerNumSteps, simulationSteps);
		cout << algoRunner.getAlgoName() << ": " << house.getFileName() << ":" << score << endl;
		scoreMgr.updateScore(algoRunner.getAlgoName(), FileUtils::getFileNameNoExt(house.getFileName()), score);
	}
}


void HouseSimulation::updateOnSuccessfulAlgo(AlgorithmRunner& successAlgorithmRunner, list<AlgorithmRunner>& algoRunnerList){
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
			updateAboutToFinish(algoRunnerList);
		}

	}
}

void HouseSimulation::updateAboutToFinish(list<AlgorithmRunner>& algoRunnerList){

	isUpdatedAboutToFinish = true;
	// update steps remaining for simulator
	numStepsRemaining = min(maxStepsAfterWinner, numStepsRemaining);
	// update steps remaining for each algorithm
	for (AlgorithmRunner& algorithmRunner : algoRunnerList){
		algorithmRunner.updateStepsRemainingOnWinner(numStepsRemaining);
	}
}


