#include "AlgorithmRunner.h"
#include <tuple>
#include <iostream>
using namespace std;

// initialization of static mambers TODO: should it be done here?
int AlgorithmRunner::currHouseTotDirt = 0;
map<string, int> AlgorithmRunner::config;

AlgorithmRunner::AlgorithmRunner(AbstractAlgorithm* a):
		roboti(-1), robotj(-1), batteryLevel(0),  numSteps (0), dirtCollected(0),
		isFinished(false), algoPositionInCompetition(-1), finishState(SimulationFinishState::NoMoreSteps)
{
	algorithm = a;
}

AlgorithmRunner::~AlgorithmRunner() {
	// TODO Auto-generated destructor stub
}

void AlgorithmRunner::resetCommonDataForNewHouse(const House& house)
{
	currHouseTotDirt = house.calcDirtLevel();
}

void AlgorithmRunner::resetRunnerForNewHouse(const House& house, int currHouseDocki, int currHouseDockj){
	setCurrHouse(house); // copy the house info using the = operator

	// set robot location
	roboti = currHouseDocki;
	robotj = currHouseDockj;

	dirtCollected = 0;
	numSteps = 0;
	batteryLevel = AlgorithmRunner::config["BatteryCapacity"];

	algoPositionInCompetition = -1;
	isFinished = false;
	finishState = SimulationFinishState::NoMoreSteps;
}

bool AlgorithmRunner::isHouseCleanAndRobotInDock(){
	return (dirtCollected == AlgorithmRunner::currHouseTotDirt
			&& currHouse.getHouseMatrix()[roboti][robotj] == 'D');
}

bool AlgorithmRunner::isBatteryConsumedAndRobotNotInDock(){
	bool isBatteryConsumed = (batteryLevel == 0) ? true:false;
	return (isBatteryConsumed && !isRobotInDock());
}

bool AlgorithmRunner::isRobotInDock(){
	return (currHouse.getHouseMatrix()[roboti][robotj] == 'D');
}


bool AlgorithmRunner::getStepAndUpdateIfLegal(){
	int stepi = roboti, stepj = robotj;
	char movePlaceVal;
	// get the direction from the algorithm
//	cout << "curr location i: " << roboti  <<" curr location j: " << robotj << endl;
	Direction direction = algorithm->step(); // TODO: fix problematic line

    switch(direction) {
		case Direction::East:
			stepj += 1;
			break;
		case Direction::West:
			stepj -= 1;
			break;
		case Direction::South:
			stepi += 1;
			break;
		case Direction::North:
			stepi -= 1;
			break;
		case Direction::Stay:
			break;
		}

    // check if the direction is legal
    if (!isLegalStep(stepi, stepj)){
    	return false;
    }

    // step is legal - update fields:
    // update robot location in runner and sensor
    roboti = stepi, robotj = stepj;
    movePlaceVal = currHouse.getHouseMatrix()[roboti][robotj];

    // update num steps
    numSteps += 1;

    if (movePlaceVal == 'D'){
    	batteryLevel = min(batteryLevel+config.find("BatteryRechargeRate")->second, config.find("BatteryCapacity")->second);
    }
    else{
    	batteryLevel = max(0, batteryLevel-config.find("BatteryConsumptionRate")->second);
    	if (movePlaceVal > '0' && movePlaceVal <= '9'){ // new place is dirty - update dirt
    		currHouse.getHouseMatrix()[stepi][stepj] -= 1;
    		dirtCollected += 1;
    	}
    }
    return true;
}

void AlgorithmRunner::updateStepsRemainingOnWinner(int numStepsRemaining){
	algorithm->aboutToFinish(numStepsRemaining);
}

bool AlgorithmRunner::isLegalStep(int stepi, int stepj){
	char suggestedPlaceVal = currHouse.getHouseMatrix()[stepi][stepj];
//	cout << "step i: " << stepi << " stepj: " << stepj << endl;
//	cout << "suggested place val: " <<  suggestedPlaceVal << endl;
	return (suggestedPlaceVal != 'W' &&
			stepi >= 0 && stepj >= 0 &&
			stepi < currHouse.getRows() && stepj < currHouse.getCols());
}

void AlgorithmRunner::updateCurrHouseScoreInList(const int winnerNumSteps, const int simulationSteps){
	int currHouseScore;
	if (finishState == SimulationFinishState::IllegalMove){
		currHouseScore = 0;
	}
	else {
		int positionInCompetition = getPositionInCompetitionForScore();
//		cout << "positionInCompetition " << positionInCompetition << endl;
//		cout << "winnerNumSteps " << winnerNumSteps << endl;
//		cout << "numSteps " << numSteps << endl;
//		cout << "curr house tot dirt "<< AlgorithmRunner::currHouseTotDirt << endl;
//		cout << "dirtCollected" << dirtCollected << endl;
//		cout << "isRobotindoc" << (isRobotInDock() ? 50 : -200) << endl;
		if (finishState == SimulationFinishState::OutOfBattery){
			numSteps = simulationSteps;
		}
		currHouseScore = max(0,
							max(2000,
							max(-(positionInCompetition - 1)*50,
							max((winnerNumSteps - numSteps)*10,
							max(-(AlgorithmRunner::currHouseTotDirt - dirtCollected)*3,
									isRobotInDock() ? 50 : -200)))));
	}
//	cout << "currhousescore" << currHouseScore << endl;
	housesScore.push_back(currHouseScore);
}

int AlgorithmRunner::getPositionInCompetitionForScore(){
	if (!(finishState == SimulationFinishState::Success)){
		return 10;
	}
	return min(algoPositionInCompetition, 4);
}

void AlgorithmRunner::setSensorForAlgorithm(){
	algorithm->setSensor(sensor);
	sensor.setSensorHouse(&currHouse);
	sensor.setRobotiPrt(&roboti);
	sensor.setRobotjPtr(&robotj);
}
