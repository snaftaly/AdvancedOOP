#include "Simulator.h"
#include "_305220980_A.h"

//C'tor implementation
Simulator::Simulator(const string& configPath, const string& housesPath ):
	houseMgr(housesPath), confMgr(false), initSuccessfull(true)
{
	// check if there are valid houses
	if (houseMgr.getNumValidHouses() == 0){
		initSuccessfull = false;
		return;
	}

	// get configurations from file
	if (!confMgr.loadFromFile(configPath)){
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
		algorithmRunnerList.emplace_back(AlgorithmRunner(algo));
	}
	for (AlgorithmRunner& algorithmRunner : algorithmRunnerList){
		algorithmRunner.setSensorForAlgorithm();
	}
}

void Simulator::runSimulation(){

	for (const House& house : houseMgr.getHouses()){

		int maxSteps = confMgr.getConfs().find("MaxSteps")->second;
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
		    cout << "\033[2J\033[1;1H"; // clear screen

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

void Simulator::printAlgosScores(){
	for (AlgorithmRunner& algoRunner : algorithmRunnerList){
		for (list<int>::iterator housesScoreitr = algoRunner.getHousesScore().begin();
				housesScoreitr != algoRunner.getHousesScore().end(); housesScoreitr++){
			cout << *housesScoreitr << endl;
		}
		// This will be used when we will have different houses
		//		for (const House& house : houseMgr.getHouses()){
		////			cout << '[' << house.getName() << ']' << '\t' << *itr << endl;
		//			++housesScoreitr;
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
	algorithms.emplace_back(new _305220980_A());
}


