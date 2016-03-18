/*
 * Simulator.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: noaleibo1
 */

#include "Simulator.h"

//C'tor implementation
Simulator::Simulator(std::list<House>& h, std::list<AbstractAlgorithm*>& a, map<string, int>& c){
	houses = h;
	algorithms = a;
	configs = c;
	createAlgorithmRunnerList();

}

//D'tor implementation
Simulator::~Simulator() {
	// TODO Auto-generated destructor stub
}


void Simulator::createAlgorithmRunnerList(){
	for (AbstractAlgorithm* algo : algorithms){
		algorithmRunnerList.emplace_back(AlgorithmRunner(algo));
	}
}

void Simulator::runSimulation(){

	for (const House& house : houses){
		setHouseForEachAlgorithmRunner(house);
	}

}

void Simulator::setHouseForEachAlgorithmRunner(const House& house) const{
	for (AlgorithmRunner& algoRunner : algorithmRunnerList){
		algoRunner.setCurrHouse(house);
	}
}





