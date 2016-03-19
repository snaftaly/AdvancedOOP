#include "Simulator.h"

//C'tor implementation
Simulator::Simulator(const string& iniPath, const string& housesPath ): confMgr(iniPath), houseMgr(housesPath){
	// TODO insert simple algorithm * to algorithms

	// TODO: should we use the hard coded configs (empty c'tor)?

	createAlgorithmRunnerList();
	AlgorithmRunner::setConfig(configs);

}

//D'tor implementation
Simulator::~Simulator() {
	// TODO Auto-generated destructor stub
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
		numAlogsFinished = 0;
		lastSuccessfullAlgoRank = -1;
		winnerNumSteps = confMgr.getConfs()["MaxSteps"];

		// set common data about the house for the algorithms
		AlgorithmRunner::resetCommonDataForNewHouse(house);
		// set the current house for the algorithm runner;
		setHouseForEachAlgorithmRunner(house);

		while (numAlogsFinished < algorithmRunnerList.size()){ // && stepsToRun > 0
			for (AlgorithmRunner& algorithmRunner : algorithmRunnerList){
				if (algorithmRunner.getIsFinished()){
					continue;
				}
				// check if the algorithm has finished
				if (algorithmRunner.isHouseCleanAndRobotInDock()){
					// TODO: think about it - this part is done in case the house is already clean in the first place
					updateOnSuccessfulAlgo(algorithmRunner);
				}
				else if (algorithmRunner.isBatteryConsumedAndRobotNotInDock()){ // check if no more battery
					algorithmRunner.setIsFinished(true);
					numAlogsFinished++;
				}
				else if(algorithmRunner.isNoMoreStepsToRun()){ // check if no more steps to run
					algorithmRunner.setIsFinished(true);
					numAlogsFinished++;
				}
				else {
					bool isMadeLegalMove = algorithmRunner.getStepAndUpdateIfLegal();
					if (!isMadeLegalMove){
						algorithmRunner.setIsFinished(true);
						numAlogsFinished++;
					}
					else { // move is legal
						// TODO: check if finished successfully also here?
						if (algorithmRunner.isHouseCleanAndRobotInDock()){
							updateOnSuccessfulAlgo(algorithmRunner);
						}

					}
				}
			}
			for (AlgorithmRunner& algoRunner : algorithmRunnerList){
				algoRunner.updateCurrHouseScoreInList(winnerNumSteps);
			}
			// end of while - update algos scores here?
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

void Simulator::updateOnSuccessfulAlgo(AlgorithmRunner& algorithmRunner){
	algorithmRunner.setIsFinished(true);
	algorithmRunner.setAlgoRankInCompetition(++lastSuccessfullAlgoRank);
	numAlogsFinished++;
	if (lastSuccessfullAlgoRank == 1){ // this algo is the first to win fpr the house
		// update winner num steps
		winnerNumSteps = winnerNumSteps - algorithmRunner.getStepsRemaining();

		// update steps to run TODO: do we need this line?
		// stepsToRun = confMgr.getConfs()["MaxStepsAfterWinner"] < stepsToRun ? confMgr.getConfs()["MaxStepsAfterWinner"] : stepsToRun;

		// update other algos with result
		for (AlgorithmRunner& algorithmRunner : algorithmRunnerList){
			algorithmRunner.updateStepsRemainingOnWinner();
		}

	}
}




