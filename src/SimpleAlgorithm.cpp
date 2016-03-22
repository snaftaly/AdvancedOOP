#include "SimpleAlgorithm.h"

SimpleAlgorithm::SimpleAlgorithm() : sensor(NULL) {
	// TODO Auto-generated constructor stub

}

SimpleAlgorithm::~SimpleAlgorithm() {
	// TODO Auto-generated destructor stub
}

void SimpleAlgorithm::setSensor(const AbstractSensor& sensor){
	this->sensor = &sensor; // passing the pointer to sensor
}

void SimpleAlgorithm::setConfiguration(std::map<std::string, int> config){
	configs = config;
}


Direction SimpleAlgorithm::step(){ //TODO: See examples from class.
	int i;
	do {
		i = rand() % 5;
	}
	while (i != 4 && sensor->sense().isWall[i] == true);
	switch (i){
	case 0:
		return Direction::East;
		break;
	case 1:
		return Direction::West;
		break;
	case 2:
		return Direction::South;
		break;
	case 3:
		return Direction::North;
		break;
	case 4:
		return Direction::Stay;
	}
}

void SimpleAlgorithm::aboutToFinish(int stepsTillFinishing){

}
