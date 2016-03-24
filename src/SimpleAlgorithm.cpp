#include "SimpleAlgorithm.h"
#include <list>

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
	std::list<int> possibleSteps = getPossibleSteps();
	i = rand() % possibleSteps.size();
	return (Direction)i;
//	do {
//		i = rand() % 5;
//	}
//	while (i != 4 && sensor->sense().isWall[i] == true);
//	return (Direction)i;
//	switch (i){
//	case 0:
//		return Direction::East;
//		break;
//	case 1:
//		return Direction::West;
//		break;
//	case 2:
//		return Direction::South;
//		break;
//	case 3:
//		return Direction::North;
//		break;
//	case 4:
//		return Direction::Stay;
//	}
}

std::list<int> SimpleAlgorithm::getPossibleSteps(){
	std::list<int> possibleSteps;
	possibleSteps.emplace_back(0);
	int i;
	bool isWallArray = sensor->sense().isWall;
	for (i=0; i<4; i++){
		if (isWallArray[i] == false){
			possibleSteps.emplace_back(i);
		}
	}
	return possibleSteps;
}

void SimpleAlgorithm::aboutToFinish(int stepsTillFinishing){

}
