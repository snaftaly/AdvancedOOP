/*
 * 305220980A.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: noaleibo1
 */

#include "_305220980_A.h"

_305220980_A::_305220980_A(): sensor(NULL), isAboutToFinish(false), stepsUntillFinishing(-1) {
	// TODO Auto-generated constructor stub
}

_305220980_A::~_305220980_A() {
	// TODO Auto-generated destructor stub
}

void _305220980_A::setSensor(const AbstractSensor& s){
	this->sensor = &s; // passing the pointer to sensor

	//Reset previous knowledge.
	isAboutToFinish = false;
	stepsUntillFinishing = -1;

	//Empty stack of previous steps
	std::stack<Direction> empty;
	std::swap(previousSteps, empty);
}

void _305220980_A::setConfiguration(std::map<std::string, int> config){
	configs = config;
}


Direction _305220980_A::step(){
	int i = 0;
	Direction nextStep = getStep();
	return nextStep; //Direction::East;
}

Direction _305220980_A::getStep(){

	Direction nextStep;

	//The robot is heading back to the docking station.
	if(isAboutToFinish){
		if (!previousSteps.empty()){
			nextStep = previousSteps.pop();
		}
		else{
			nextStep = Direction::Stay;
		}
		return nextStep;
	}

	if (sensor->sense().dirtLevel > 0){
		nextStep = Direction::Stay;
	}

	else{
		//East, West, South, North, Stay
		int currDirection = 0;
		while (currDirection < 4 && sensor->sense().isWall[currDirection]){
			++currDirection;
		}
		nextStep = (Direction)currDirection;
	}

	//Adds only steps that are not 'stay'
	switch (nextStep){
		case Direction::East:
			previousSteps.push(Direction::West);
			break;
		case Direction::West:
			previousSteps.push(Direction::East);
			break;
		case Direction::North:
			previousSteps.push(Direction::South);
			break;
		case Direction::South:
			previousSteps.push(Direction::North);
			break;
		case Direction::Stay:
			break;
	}

	if (nextStep != Direction::Stay){

		previousSteps.push(nextStep);
	}
	return nextStep;
}

void _305220980_A::aboutToFinish(int stepsTillFinishing){
	isAboutToFinish = true;
	stepsUntillFinishing = stepsTillFinishing;
}
