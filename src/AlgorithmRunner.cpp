/*
 * AlgorithmRunner.cpp
 *
 *  Created on: Mar 17, 2016
 *      Author: noaleibo1
 */

#include "AlgorithmRunner.h"

AlgorithmRunner::AlgorithmRunner(AbstractAlgorithm* a): houseTotalDirtLevel(-1), isRunning(true), numOfSteps(0), dirtCollected(0) {
	algorithm = a;
	algorithm->setSensor(sensor);
	sensor.setSensorHouse(&currHouse);
	//TODO refer to battery.

}

AlgorithmRunner::~AlgorithmRunner() {
	// TODO Auto-generated destructor stub
}

void AlgorithmRunner::updateHouseAndInfo(const House& house){
	setCurrHouse(house);
	int roboti;
	int robotj;
	std::tie(houseTotalDirtLevel, roboti, robotj) = currHouse.calculateHouseDirtLevelAndDocking();
	isRunning = true;
	sensor.setRobotLocation(roboti, robotj);
}
