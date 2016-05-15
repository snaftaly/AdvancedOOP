#include "_305220980_D.h"


Direction _305220980_D::step(Direction prevStep) {
	return Direction::Stay;
}

void _305220980_D::setSensor(const AbstractSensor& sensor) {

}

// TODO:
// 1) use amir's code for getting the data of prev step, including how to get back to docking
// 2) use bfs to get closest un-reveald path (using parent) OR understand amir's code for getting next step.
// 		but use a different order for searching for each of the algos
// 3) use our logic for returning back to the house - but remember to use the distance from docking from amir's code
