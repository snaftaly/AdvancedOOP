#include "_305220980_D.h"
#include "BatteryManager.h"

void _305220980_D::setSensor(const AbstractSensor& sensor) {
	this->sensor = &s; // passing the pointer to sensor

	//Reset previous knowledge.
	stepsUntillFinishing = -1;
	batteryMng.resetBattery();
	isGoingBack = false;
	prevStepFromAlgo = Direction::Stay;

	//Empty stack of previous steps - TODO-Check.
	std::stack<Direction> empty;
	std::swap(previousSteps, empty);
}

// TODO:
// 1) use amir's code for getting the data of prev step, including how to get back to docking
// 2) use bfs to get closest un-reveald path (using parent) OR understand amir's code for getting next step.
// 		but use a different order for searching for each of the algos
// 3) use our logic for returning back to the house - but remember to use the distance from docking from amir's code


Direction _305220980_D::step(Direction prevStep) {
	return Direction::Stay;
}
