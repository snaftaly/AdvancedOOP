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

	bool isStillInCurrentHouse;
	for (const House& house : houseMgr.getHouses()){
		// set the current house for the algorithm runner;
		setHouseForEachAlgorithmRunner(house);

		while (true){
			for (AlgorithmRunner algorithmRunner : algorithmRunnerList){

			}
		}
	}

}

void Simulator::setHouseForEachAlgorithmRunner(const House& house) const{
	for (AlgorithmRunner& algoRunner : algorithmRunnerList){
		algoRunner.resetRunnerForNewHouse(house);
	}
}





