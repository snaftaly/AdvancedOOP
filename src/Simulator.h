/*
 * Simulator.h
 *
 *  Created on: Mar 11, 2016
 *      Author: noaleibo1
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "House.h"
#include <list>
#include <map>
#include <string>

class Simulator {

	std::list<House> houses;
	std::list<AbstractAlgorithm*> algorithms;
	map<string, int> configs;
	std::list<AlgorithmRunner> algorithmRunnerList;

public:

	//C'tor signature
	Simulator(std::list<House>& h, std::list<AbstractAlgorithm*>& a, map<string, int>& configs);

	//D'tor signature
	~Simulator();

	void createAlgorithmRunnerList();

	//Function to run all algorithms on all houses
	void runAlgorithmsOnHouses();

	//Function to run actual simulation
	void runSimulation();

	void setHouseForEachAlgorithmRunner(const House& house) const;


};

#endif /* SIMULATOR_H_ */
