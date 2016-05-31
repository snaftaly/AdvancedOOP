/*
 * HouseSimulation.cpp
 *
 *  Created on: May 5, 2016
 *      Author: snaftaly
 */

#include "HouseSimulation.h"
#include "FileUtils.h"
#include "Encoder.h"

using namespace std;

HouseSimulation::HouseSimulation() {
	// TODO Auto-generated constructor stub

}

HouseSimulation::~HouseSimulation() {
	// TODO Auto-generated destructor stub
}

void HouseSimulation::runSimulationForHouse(AlgorithmManager& algoMgr, ScoreManager& scoreMgr, VideoManager& videoMgr, const House& house,
		list<AlgorithmRunner>& algoRunnerList, const int _maxStepsAfterWinner, bool doVideo){
	int maxSteps = house.getMaxSteps(); // no need to save

	currSuccessfullAlgoPosition = 1;
	winnerNumSteps = maxSteps;
	numStepsRemaining = maxSteps;
	numAlogsRunning = algoRunnerList.size();
	simulationSteps = 0;
	isThereAWinner = false;
	isUpdatedAboutToFinish = false;
	maxStepsAfterWinner = _maxStepsAfterWinner;

	// if doVideo - create a directory for each algo and put the initial house state in it
	if (doVideo){
		for (AlgorithmRunner& algorithmRunner : algoRunnerList){
			algorithmRunner.setDoVideo(true);
			string imagesDirPath = algorithmRunner.getAlgoName() + "_" + FileUtils::getFileNameNoExt(house.getFileName());
			if (!FileUtils::createDirectoryIfNotExists(imagesDirPath)){
				algorithmRunner.setDoVideo(false);
				videoMgr.addDirCreationError(algorithmRunner.getAlgoName(), FileUtils::getFileNameNoExt(house.getFileName()), imagesDirPath);
			}
			else {
				algorithmRunner.setImageDir(imagesDirPath);
				algorithmRunner.addStepImage(videoMgr);
			}
		}
	}

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
				bool isMadeLegalMove = algorithmRunner.getStepAndUpdateIfLegal(videoMgr);
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
	int algoScore;
	for (AlgorithmRunner& algoRunner : algoRunnerList){
		// currSuccessfullAlgoPosition now has the position of the unsuccessful algos
		algoScore = scoreMgr.calcScore(algoRunner.isMadeIllegalMove(), algoRunner.getScoreParams(winnerNumSteps, simulationSteps, currSuccessfullAlgoPosition));
		scoreMgr.updateScore(algoRunner.getAlgoName(), FileUtils::getFileNameNoExt(house.getFileName()), algoScore);
		if (doVideo){
			// update error list if there was an error creating some image
			if (algoRunner.getFailedImagesCounter() > 0){
				videoMgr.addImageCreationErrors(algoRunner.getAlgoName(), FileUtils::getFileNameNoExt(house.getFileName()), algoRunner.getFailedImagesCounter());
			}
			// create the video
			string imagesExpression = algoRunner.getImagesDir() + "/image%50d.jpg";
			if (!Encoder::encode(imagesExpression, algoRunner.getImagesDir() + ".mpg")){
				videoMgr.addVideoCreationError(algoRunner.getAlgoName(), FileUtils::getFileNameNoExt(house.getFileName()));
			}
			// remove the directory
			if(!FileUtils::removeDir(algoRunner.getImagesDir())){
				// TODO: add dir removal errors?
			}
		}
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


