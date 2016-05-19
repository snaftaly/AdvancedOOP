#include "_305220980_B.h"

Direction _305220980_B::step(Direction prevStep) {
	// algo is using caution
	return getStepAndUpdatePrevStep(possibleMoves, prevStep, true);
}

REGISTER_ALGORITHM (_305220980_B)
