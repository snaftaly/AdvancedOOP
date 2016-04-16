/*
 * 305220980B.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: noaleibo1
 */

#include "_305220980_C.h"

_305220980_C::_305220980_C(): sensor(NULL), stepsUntillFinishing(-1) {
	// TODO Auto-generated constructor stub
}

_305220980_C::~_305220980_C() {
	// TODO Auto-generated destructor stub
}

void _305220980_C::setSensor(const AbstractSensor& s){
	this->sensor = &s; // passing the pointer to sensor

	//Reset previous knowledge.
	stepsUntillFinishing = -1;

	//Empty stack of previous steps - TODO-Check.
	std::stack<Direction> empty;
	std::swap(previousSteps, empty);
}

void _305220980_C::setConfiguration(std::map<std::string, int> config){
	configs = config;
}


Direction _305220980_C::step(){
	Direction nextStep = getStep();
	return nextStep; //Direction::East;
}

Direction _305220980_C::getStep(){

	Direction nextStep;

	//The robot is heading back to the docking station.
	if(stepsUntillFinishing != -1){
		if (!previousSteps.empty()){
			nextStep = previousSteps.top();
			previousSteps.pop();
		}
		else{
			nextStep = Direction::Stay;
		}
		return nextStep;
	}

//	cout << "!!!!!!!dirtLevel!!!!!" << endl;
//	cout << sensor->sense().dirtLevel << endl; TODO

	int dirtLevel = sensor->sense().dirtLevel;
	if (dirtLevel > 0){
		nextStep = Direction::Stay;
	}

	else{
		//East, West, South, North, Stay
		std::vector<Direction> possibleMoves {Direction::South,Direction::West,Direction::East,Direction::North,Direction::Stay};
		Direction currDirection;
		int i = 0;
		while ((currDirection = possibleMoves[i]) != Direction::Stay && sensor->sense().isWall[(int)currDirection]){
			i++;
		}
		nextStep = currDirection;
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

void _305220980_C::aboutToFinish(int stepsTillFinishing){
	stepsUntillFinishing = stepsTillFinishing;
}
