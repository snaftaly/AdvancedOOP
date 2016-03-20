#include "AlgorithmRunner.h"
#include <tuple>

AlgorithmRunner::AlgorithmRunner(AbstractAlgorithm* a):
		stepsRemaining(-1), numSteps (0), dirtCollected(0), isFinished(false),
		roboti(-1), robotj(-1), batteryLevel(0), algoRankInCompetition(-2) {

	algorithm = a;
	algorithm->setSensor(sensor);
	sensor.setSensorHouse(&currHouse);
	sensor.setRobotiPrt(&roboti);
	sensor.setRobotjPtr(&robotj);
}

AlgorithmRunner::~AlgorithmRunner() {
	// TODO Auto-generated destructor stub
}

void AlgorithmRunner::resetCommonDataForNewHouse(const House& house)
{
	currHouseTotDirt = house.calcDirtLevel();
	std::tie(currHouseDocki, currHouseDockj) = house.getHouseDockPlace();
}

void AlgorithmRunner::resetRunnerForNewHouse(const House& house){
	setCurrHouse(house); // copy the house info using the = operator

	// set robot location
	roboti = AlgorithmRunner::currHouseDocki;
	robotj = AlgorithmRunner::currHouseDockj;

	dirtCollected = 0;
	numSteps = 0;
	stepsRemaining = AlgorithmRunner::config["MaxSteps"];
	batteryLevel = AlgorithmRunner::config["BatteryCapacity"];

	algoRankInCompetition = -1;
	isFinished = false;
}

bool AlgorithmRunner::isHouseCleanAndRobotInDock(){
	return (dirtCollected == AlgorithmRunner::currHouseTotDirt
			&& AlgorithmRunner::currHouseDocki == roboti
			&& AlgorithmRunner::currHouseDocki == robotj);
}

bool AlgorithmRunner::isBatteryConsumedAndRobotNotInDock(){
	// TODO: handle this
	return false;
}

bool AlgorithmRunner::isNoMoreStepsToRun(){
	return stepsRemaining == 0;
}

bool AlgorithmRunner::isBackInDocking(){
	return (AlgorithmRunner::currHouseDocki == roboti && AlgorithmRunner::currHouseDockj == robotj);
}


bool AlgorithmRunner::getStepAndUpdateIfLegal(){
	int stepi = roboti, stepj = robotj;
	char movePlaceVal;
	// get the direction from the algorithm
	Direction direction = algorithm->step();
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

    sensor.setRobotLocation(roboti, robotj);
    // update num steps
    stepsRemaining -= 1;
    numSteps += 1;

    if (movePlaceVal == 'D'){
    	// TODO: increase battery level (unless fully charged)
    }
    else{
    	// TODO: decrease battery level
    	if (movePlaceVal != ' ' && movePlaceVal != '0'){ // new place is dirty - update dirt
    		currHouse.getHouseMatrix()[stepi][stepj] -= 1;
    		dirtCollected += 1;
    	}
    }
    return true;
}

void AlgorithmRunner::updateStepsRemainingOnWinner(){
	stepsRemaining = min(stepsRemaining, AlgorithmRunner::config["MaxStepsAfterWinner"]);
	algorithm->aboutToFinish(stepsRemaining);
}

bool AlgorithmRunner::isLegalStep(int stepi, int stepj){
	char suggestedPlaceVal = currHouse.getHouseMatrix()[stepi][stepj];
	if (suggestedPlaceVal == 'W' ||
			stepi < 0 || stepj < 0 ||
			stepi >= currHouse.getRows() || stepj >= currHouse.getCols()){
		// step is illegal
		return false;
	}
	return true;
}

void AlgorithmRunner::updateCurrHouseScoreInList(const int winnerNumSteps){
	int positionInCompetition = getPositionInCompetition();
	int currHouseScore = max(0,
						max(2000,
						max(-(positionInCompetition - 1)*50,
						max(-(winnerNumSteps - numSteps)*10,
						max(+(AlgorithmRunner::currHouseTotDirt - dirtCollected)*3,
								isBackInDocking() ? 50 : -200)))));

	housesScore.push_back(currHouseScore);
}

int AlgorithmRunner::getPositionInCompetition(){
	if (algoRankInCompetition == -1){
		return 10;
	}
	return min(algoRankInCompetition, 4);
}
