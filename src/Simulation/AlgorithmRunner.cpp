#include <tuple>
#include <iostream>
#include <unistd.h> // for usleep
#include "AlgorithmRunner.h"
#include "Montage.h"
using namespace std;

// initialization of static members
map<string, int> AlgorithmRunner::config;

AlgorithmRunner::AlgorithmRunner(unique_ptr<AbstractAlgorithm>& _algorithm, string algoName):
		algoName(algoName), roboti(-1), robotj(-1), batteryLevel(0),  numSteps (0),
		dirtCollected(0), prevStep(Direction::Stay), algoPositionInCompetition(-1), simulationState(SimulationState::Running),
		doVideo(false), imagesCounter(0), failedImagesCounter(0)
{
	algorithm = std::move(_algorithm);
	setSensorForAlgorithm();
}

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


bool AlgorithmRunner::getStepAndUpdateIfLegal(VideoManager& videoMgr){
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
    if (doVideo){
    	addStepImage(videoMgr);
    }
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
	sensor.setRobotiPtr(&roboti);
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
    usleep(50000);
    currHouse.getHouseMatrix()[roboti][robotj] = currChar;
}

void AlgorithmRunner::addStepImage(VideoManager& videoMgr){
	// put the robot temporarily in the house matrix
    char currChar = currHouse.getHouseMatrix()[roboti][robotj];
    currHouse.getHouseMatrix()[roboti][robotj] = 'R';
    vector<string> tilesForStep;

	for (int i = 0; i< currHouse.getRows() ; i++){
		for (int j = 0; j < currHouse.getCols(); j++){

            if (currHouse.getHouseMatrix()[i][j] == ' '){
            	tilesForStep.push_back("0");
            }
            else {
            	tilesForStep.push_back(string() + currHouse.getHouseMatrix()[i][j]);
            }
		}
	}
	// put the curr char back in the house
    currHouse.getHouseMatrix()[roboti][robotj] = currChar;
    string counterStr = to_string(imagesCounter++);
    string composedImage = imagesDir + "/image" + string(5-counterStr.length(), '0') + counterStr + ".jpg";
    if (!Montage::compose(tilesForStep, currHouse.getCols(), currHouse.getRows(), composedImage)){
    	failedImagesCounter++;
    }
}
