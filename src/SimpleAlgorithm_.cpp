#include "SimpleAlgorithm_.h"
#include <vector>

SimpleAlgorithm::SimpleAlgorithm() : sensor(NULL) {
}

SimpleAlgorithm::~SimpleAlgorithm() {

}

void SimpleAlgorithm::setSensor(const AbstractSensor& s){
	this->sensor = &s; // passing the pointer to sensor
}

void SimpleAlgorithm::setConfiguration(std::map<std::string, int> config){
	configs = config;
}


Direction SimpleAlgorithm::step(){
//	int i = 0;
	std::vector<Direction> possibleSteps = getPossibleSteps();
//	i = rand() % possibleSteps.size();

	return Direction::East;//possibleSteps[i]; //Direction::East;
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

extern "C" {
AbstractAlgorithm *maker(){
   return new SimpleAlgorithm;
}
class proxy {
public:
   proxy(){
      // register the maker with the factory
      factory["SimpleAlgorithm_"] = maker;
   }
};
// our one instance of the proxy
proxy p;
}
