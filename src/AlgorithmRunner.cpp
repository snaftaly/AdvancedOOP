#include "AlgorithmRunner.h"

AlgorithmRunner::AlgorithmRunner(AbstractAlgorithm* a):
		houseTotalDirtLevel(-1), numOfSteps(0), dirtCollected(0),
		roboti(-1), robotj(-1), batteryConsumed(0), algoRankInCompetition(-2) {

	algorithm = a;
	algorithm->setSensor(sensor);
	sensor.setSensorHouse(&currHouse);
}

AlgorithmRunner::~AlgorithmRunner() {
	// TODO Auto-generated destructor stub
}

void AlgorithmRunner::resetRunnerForNewHouse(const House& house){
	setCurrHouse(house); // copy the house info using the = operator
	int roboti;
	int robotj;

	sensor.setRobotLocation(roboti, robotj);
}
