#ifndef _305220980_B_H_
#define _305220980_B_H_

#include "../Common/Direction.h"
#include "../Common/AbstractSensor.h"
#include "GenericAlgorithm.h"

class _305220980_B: public GenericAlgorithm {
	int counter;
	std::vector<Direction> possibleMoves;
public:

	 _305220980_B(): GenericAlgorithm(), counter(0){
		 possibleMoves = {Direction::East};
	 }

	 virtual Direction step(Direction prevStep) override;

	 virtual void setSensor(const AbstractSensor& sensor) override;
};

#endif /* 305220980B_H_ */
