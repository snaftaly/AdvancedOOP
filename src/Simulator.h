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
#include "AlgorithmRunner.h"
#include <list>
#include <map>
#include <string>

class Simulator {
	HouseManager houseMgr;
	ConfigManager confMgr;

	std::list<AbstractAlgorithm*> algorithms;
	std::list<AlgorithmRunner> algorithmRunnerList;

public:

	//C'tor signature
	Simulator(const std::string& iniPath, const std::string& housesPath);

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
