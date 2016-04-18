/*
 * 305220980A.cpp
 *
 *  Created on: Apr 11, 2016
 *      Author: noaleibo1
 */

#include "_305220980_A.h"

_305220980_A::_305220980_A(): {
	// TODO Auto-generated constructor stub
}

_305220980_A::~_305220980_A() {
	// TODO Auto-generated destructor stub
}

Direction _305220980_A::getStep(){

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
	}

	else{
		int dirtLevel = sensor->sense().dirtLevel;
		if (dirtLevel > 0){
			nextStep = Direction::Stay;
		}

		else{
			std::vector<Direction> possibleMoves {Direction::East,Direction::North,Direction::South,Direction::West,Direction::Stay};
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
	}

	return nextStep;
}
