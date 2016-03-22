#include "Simulator.h"

//C'tor implementation
Simulator::Simulator(const string& iniPath, const string& housesPath ):
	confMgr(iniPath), houseMgr(housesPath)
{
	// TODO insert simple algorithm * to algorithms

	createAlgorithmRunnerList();
	AlgorithmRunner::setConfig(configs);

}

//D'tor implementation
Simulator::~Simulator() {
	// TODO Auto-generated destructor stub  - maybe move to h file?
}


void Simulator::createAlgorithmRunnerList(){
	for (AbstractAlgorithm* algo : algorithms){
		// set the configurations for the algorithm
		algo->setConfiguration(confMgr.getConfs());

		// put the algorithm in the algoRunner list
		algorithmRunnerList.emplace_back(AlgorithmRunner(algo));
	}
}

void Simulator::runSimulation(){

	for (const House& house : houseMgr.getHouses()){

		currSuccessfullAlgoPosition = 0;
		winnerNumSteps = numStepsRemaining = confMgr.getConfs()["MaxSteps"];
		numAlogsRunning = algorithmRunnerList.size();

		// set common data about the house for the algorithms
		AlgorithmRunner::resetCommonDataForNewHouse(house);

		// set the current house for the algorithm runner;
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
					// TODO: think about it - maybe it's enough to update it here, maybe not
					// this part is done in case the house is already clean before making any step
					updateOnSuccessfulAlgo(algorithmRunner);
				}
				else if (algorithmRunner.isBatteryConsumedAndRobotNotInDock()){ // check if no more battery
					// TODO: need to print something?
					algorithmRunner.setIsFinished(true);
					numAlogsRunning--;
				}
				else {
					bool isMadeLegalMove = algorithmRunner.getStepAndUpdateIfLegal();
					if (!isMadeLegalMove){
						// TODO: how should error messages be printed
						cout << "Error: algorithm made an illegal step." << endl;
						algorithmRunner.setIsFinished(true);
						numAlogsRunning--;
					}
					else { // move is legal
						// TODO: check if finished successfully also here?
						if (algorithmRunner.isHouseCleanAndRobotInDock()){
							updateOnSuccessfulAlgo(algorithmRunner);
						}

					}
				}
			}
			// end of for loop for each algorithm - update step remaining
			numStepsRemaining--;
			currSuccessfullAlgoPosition += numSuccessfulAlgosInRound;
		}
		// end of while for the house - update algos scores
		for (AlgorithmRunner& algoRunner : algorithmRunnerList){
			algoRunner.updateCurrHouseScoreInList(winnerNumSteps);
		}
	}

}

void Simulator::printAlgosScores(){
	for (AlgorithmRunner& algoRunner : algorithmRunnerList){
		list<int>::iterator itr = algoRunner.getHousesScore().begin();
		for (House& house : houseMgr.getHouses()){
			cout << '[' << house.getName() << ']' << '\t' << *itr << endl;
			++itr;
		}
	}
};

void Simulator::setHouseForEachAlgorithmRunner(const House& house) const{
	for (AlgorithmRunner& algoRunner : algorithmRunnerList){
		algoRunner.resetRunnerForNewHouse(house);
	}
}

void Simulator::updateOnSuccessfulAlgo(AlgorithmRunner& successAlgorithmRunner){
	successAlgorithmRunner.setIsFinished(true);
	successAlgorithmRunner.setAlgoRankInCompetition(currSuccessfullAlgoPosition);
	numSuccessfulAlgosInRound++;
	numAlogsRunning--;
	if (currSuccessfullAlgoPosition == 1){ // this algo is the first to win for the house
		// update winner num steps
		winnerNumSteps =  successAlgorithmRunner.getNumSteps();

		// update steps to run to the minimum of MaxStepsAfterWinner and numStepsRemaining
		 numStepsRemaining = confMgr.getConfs()["MaxStepsAfterWinner"] < numStepsRemaining ? confMgr.getConfs()["MaxStepsAfterWinner"] : numStepsRemaining;

		// update other algos with result
		for (AlgorithmRunner& algorithmRunner : algorithmRunnerList){
			algorithmRunner.updateStepsRemainingOnWinner(numStepsRemaining);
		}

	}
}




