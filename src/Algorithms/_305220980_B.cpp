#include "_305220980_B.h"

Direction _305220980_B::step(Direction prevStep) {
	counter = (counter + 1) % 4;
	possibleMoves[0] = (Direction)counter;
	return getStepAndUpdatePrevStep(possibleMoves, prevStep);
}

void _305220980_B::setSensor(const AbstractSensor& sensor){
	counter = 0;
	possibleMoves = {Direction::East};
	GenericAlgorithm::setSensor(sensor);
}

REGISTER_ALGORITHM (_305220980_B)
