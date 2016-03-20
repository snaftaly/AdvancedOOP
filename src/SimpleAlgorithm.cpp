/*
 * simpleAlgorithm.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: noaleibo1
 */

#include "SimpleAlgorithm.h"

SimpleAlgorithm::SimpleAlgorithm() {
	// TODO Auto-generated constructor stub

}

SimpleAlgorithm::~SimpleAlgorithm() {
	// TODO Auto-generated destructor stub
}

void SimpleAlgorithm::setSensor(const Sensor& s){
	sensor = s;
}

void SimpleAlgorithm::setConfiguration(std::map<std::string, int> config){
	configs = config;
}


Direction SimpleAlgorithm::step(){
	int i = rand() % 5;
	while (i != 5 && sensor.sense().isWall[i] == true){
		int i = rand() % 5;
	}
	switch (i){
	case 0:
		return Direction::East;
		break;
	case 1:
		return Direction::North;
		break;
	case 2:
		return Direction::South;
		break;
	case 3:
		return Direction::West;
		break;
	case 5:
		return Direction::Stay;
	}
}

void SimpleAlgorithm::aboutToFinish(int stepsTillFinishing){

}
