#include "SimpleAlgorithm.h"
#include <vector>

SimpleAlgorithm::SimpleAlgorithm() : sensor(NULL) {
	// TODO Auto-generated constructor stub

}

SimpleAlgorithm::~SimpleAlgorithm() {
	// TODO Auto-generated destructor stub
}

void SimpleAlgorithm::setSensor(const AbstractSensor& s){
	this->sensor = &s; // passing the pointer to sensor
}

void SimpleAlgorithm::setConfiguration(std::map<std::string, int> config){
	configs = config;
}


Direction SimpleAlgorithm::step(){ //TODO: See examples from class.
	int i = 0;
	std::vector<Direction> possibleSteps = getPossibleSteps();
	i = rand() % possibleSteps.size();
	return possibleSteps[i]; //Direction::East;

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

std::vector<Direction> SimpleAlgorithm::getPossibleSteps(){
	std::vector<Direction> possibleSteps;
	possibleSteps.push_back(Direction::Stay);
	int i;

	for (i=0; i<4; i++){
		if (!sensor->sense().isWall[i]){
			possibleSteps.push_back((Direction)i);
		}
	}
	return possibleSteps;
}

void SimpleAlgorithm::aboutToFinish(int stepsTillFinishing){

}
