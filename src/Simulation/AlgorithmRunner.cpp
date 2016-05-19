#include "AlgorithmRunner.h"
#include <tuple>
#include <iostream>
#include <unistd.h> // for usleep
using namespace std;

// initialization of static members
map<string, int> AlgorithmRunner::config;

AlgorithmRunner::AlgorithmRunner(unique_ptr<AbstractAlgorithm>& _algorithm, string algoName):
		algoName(algoName), robotPos(), batteryLevel(0),  numSteps (0),
		dirtCollected(0), prevStep(Direction::Stay), algoPositionInCompetition(-1), simulationState(SimulationState::Running)

{
	algorithm = std::move(_algorithm);
	setSensorForAlgorithm();
}

void AlgorithmRunner::resetRunnerForNewHouse(const House& house, Point currHouseDockPos, int currHouseDirt){
	setCurrHouse(house); // copy the house info using the = operator

	// set robot location
	robotPos = currHouseDockPos;

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
			&& currHouse.getHouseMatrix()[robotPos.getY()][robotPos.getX()] == 'D');
}

bool AlgorithmRunner::isBatteryConsumedAndRobotNotInDock(){
	bool isBatteryConsumed = (batteryLevel == 0) ? true:false;
	return (isBatteryConsumed && !isRobotInDock());
}

bool AlgorithmRunner::isRobotInDock(){
	return (currHouse.getHouseMatrix()[robotPos.getY()][robotPos.getX()] == 'D');
}


bool AlgorithmRunner::getStepAndUpdateIfLegal(){
	Point suggestedPos = robotPos;
	char movePlaceVal;

	// get the direction from the algorithm
	prevStep = algorithm->step(prevStep);

	suggestedPos.move(prevStep);

    // check if the direction is legal
    if (!isLegalStep(suggestedPos.getY(), suggestedPos.getX())){
    	return false;
    }

    // step is legal - update fields:
    // update robot location in runner and sensor
    robotPos = suggestedPos;
    movePlaceVal = currHouse.getHouseMatrix()[robotPos.getY()][robotPos.getX()];

    // update num steps
    numSteps += 1;

    if (movePlaceVal == 'D'){
    	batteryLevel = min(batteryLevel+config.find("BatteryRechargeRate")->second, config.find("BatteryCapacity")->second);
    }
    else{
    	batteryLevel = max(0, batteryLevel-config.find("BatteryConsumptionRate")->second);
    	if (movePlaceVal > '0' && movePlaceVal <= '9'){ // new place is dirty - update dirt
    		currHouse.getHouseMatrix()[robotPos.getY()][robotPos.getX()] -= 1;
    		dirtCollected += 1;
    	}
    }
//    printSimulation(robotPos.getY(), robotPos.getX()); // print for tests
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


const std::map<std::string, int> AlgorithmRunner::getScoreParams(const int winnerNumSteps, const int simulationSteps,
		const int unsuccessfullAlgosPosition){
	map<string, int> scoreParams;
	int actualPositionInCompetition = simulationState == SimulationState::Success ? algoPositionInCompetition : unsuccessfullAlgosPosition;
	if (simulationState == SimulationState::OutOfBattery){
		numSteps = simulationSteps;
	}
	scoreParams["actual_position_in_competition"] = actualPositionInCompetition;
	scoreParams["simulation_steps"] = simulationSteps;
	scoreParams["winner_num_steps"] = winnerNumSteps;
	scoreParams["this_num_steps"] = numSteps;
	scoreParams["sum_dirt_in_house"] = currHouseTotDirt;
	scoreParams["dirt_collected"] = dirtCollected;
	scoreParams["is_back_in_docking"] = isRobotInDock() ? 1 : 0;
	//		//print for tests
//			cout << "algoname: " << algoName << endl;
//			cout << "actual_position_in_competition " << actualPositionInCompetition << endl;
//			cout << "winnerNumSteps " << winnerNumSteps << endl;
//			cout << "numSteps " << numSteps << endl;
//			cout << "curr house tot dirt "<< AlgorithmRunner::currHouseTotDirt << endl;
//			cout << "dirtCollected" << dirtCollected << endl;
//			cout << "isRobotindoc" << (isRobotInDock() ? 50 : -200) << endl;
//			cout << currHouse << endl;
	return scoreParams;
}

bool AlgorithmRunner::isMadeIllegalMove(){
	return simulationState == SimulationState::IllegalMove;
}

void AlgorithmRunner::setSensorForAlgorithm(){
	algorithm->setSensor(sensor);
	sensor.setSensorHouse(&currHouse);
	sensor.setRobotPosPtr(&robotPos);
}

void AlgorithmRunner::printSimulation(int stepi, int stepj){
    // only for tests
    char currChar = currHouse.getHouseMatrix()[stepi][stepj];
    cout << "\033[2J\033[1;1H"; // clear screen
    currHouse.getHouseMatrix()[robotPos.getY()][robotPos.getX()] = 'B';
    cout << currHouse << endl;
    cout << "num steps: " << numSteps  << "/" << currHouse.getMaxSteps() << endl;
    cout << "Battery: " << batteryLevel << endl;
    cout << "dirt collected: " << dirtCollected << "/" <<  AlgorithmRunner::currHouseTotDirt << endl;
    usleep(50000);
    currHouse.getHouseMatrix()[robotPos.getY()][robotPos.getX()] = currChar;
}
