#include "_305220980_A.h"

Direction _305220980_A::step(Direction prevStep) {
	// algo is not using caution
	return getStepAndUpdatePrevStep(possibleMoves, prevStep, false);
}

REGISTER_ALGORITHM (_305220980_A)
