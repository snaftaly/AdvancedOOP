#include "_305220980_B.h"

Direction _305220980_B::step(Direction prevStep) {
	counter = (counter + 1) % 4;
	possibleMoves[0] = (Direction)counter;
	return getStepAndUpdatePrevStep(possibleMoves, prevStep);
}

REGISTER_ALGORITHM (_305220980_B)
