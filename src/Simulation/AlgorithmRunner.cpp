#include "AlgorithmRunner.h"
#include <tuple>
#include <iostream>
#include <unistd.h> // for usleep
using namespace std;

// initialization of static members
//int AlgorithmRunner::currHouseTotDirt = 0;
map<string, int> AlgorithmRunner::config;

AlgorithmRunner::AlgorithmRunner(unique_ptr<AbstractAlgorithm>& _algorithm, string algoName):
		algoName(algoName), roboti(-1), robotj(-1), batteryLevel(0),  numSteps (0),
		dirtCollected(0), prevStep(Direction::Stay), algoPositionInCompetition(-1), simulationState(SimulationState::Running)
{
	algorithm = std::move(_algorithm);
	setSensorForAlgorithm();
}

//void AlgorithmRunner::resetCommonDataForNewHouse(const House& house)
//{
//	currHouseTotDirt = house.calcDirtLevel();
//}

void AlgorithmRunner::resetRunnerForNewHouse(const House& house, int currHouseDocki, int currHouseDockj, int currHouseDirt){
	setCurrHouse(house); // copy the house info using the = operator

	// set robot location
	roboti = currHouseDocki;
	robotj = currHouseDockj;

	algorithm->setSensor(sensor);//call set sensor again - reset of algo will be handled by algo

	dirtCollected = 0;
	numSteps = 0;
	batteryLevel = AlgorithmRunner::config["BatteryCapacity"];
	prevStep = Direction::Stay;
	currHouseTotDirt = currHouseDirt;

	algoPositionInCompetition = -1;
	simulationState = SimulationState::Running;
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
	prevStep = algorithm->step(prevStep);

    switch(prevStep) {
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
//    printSimulation(stepi, stepj); // print for tests
    return true;
}

void AlgorithmRunner::updateStepsRemainingOnWinner(int numStepsRemaining){
	algorithm->aboutToFinish(numStepsRemaining);
}

bool AlgorithmRunner::isLegalStep(int stepi, int stepj){
	char suggestedPlaceVal = currHouse.getHouseMatrix()[stepi][stepj];
	return (suggestedPlaceVal != 'W' &&
			stepi >= 0 && stepj >= 0 &&
			stepi < currHouse.getRows() && stepj < currHouse.getCols());
}

int AlgorithmRunner::getCurrHouseScore(const int winnerNumSteps, const int simulationSteps, const string& houseName){
	int currHouseScore;
	if (simulationState == SimulationState::IllegalMove){
		currHouseScore = 0;
	}
	else {
		int positionInCompetition = getPositionInCompetitionForScore();
		if (simulationState == SimulationState::OutOfBattery){
			numSteps = simulationSteps;
		}
		//print for tests
		cout << "algoname: " << algoName << endl;
		cout << "positionInCompetition " << positionInCompetition << endl;
		cout << "winnerNumSteps " << winnerNumSteps << endl;
		cout << "numSteps " << numSteps << endl;
		cout << "curr house tot dirt "<< AlgorithmRunner::currHouseTotDirt << endl;
		cout << "dirtCollected" << dirtCollected << endl;
		cout << "isRobotindoc" << (isRobotInDock() ? 50 : -200) << endl;
		cout << currHouse << endl;
		currHouseScore = max(0,
				2000
				- (positionInCompetition - 1)*50
				+ (winnerNumSteps - numSteps)*10
				-(AlgorithmRunner::currHouseTotDirt - dirtCollected)*3
				+ (isRobotInDock() ? 50 : -200)
				);
	}
	return currHouseScore;
//	housesScores[houseName] = currHouseScore;
}

int AlgorithmRunner::getPositionInCompetitionForScore(){
	if (!(simulationState == SimulationState::Success)){
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

void AlgorithmRunner::printSimulation(int stepi, int stepj){
    // only for tests
    char currChar = currHouse.getHouseMatrix()[stepi][stepj];
    cout << "\033[2J\033[1;1H"; // clear screen
    currHouse.getHouseMatrix()[roboti][robotj] = 'B';
    cout << currHouse << endl;
    cout << "num steps: " << numSteps  << "/" << currHouse.getMaxSteps() << endl;
    cout << "Battery: " << batteryLevel << endl;
    cout << "dirt collected: " << dirtCollected << "/" <<  AlgorithmRunner::currHouseTotDirt << endl;
    usleep(2000000);
    currHouse.getHouseMatrix()[roboti][robotj] = currChar;
}
