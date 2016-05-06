#ifndef _305220980_A_H_
#define _305220980_A_H_

#include "../Common/Direction.h"
#include "../Common/AbstractSensor.h"
#include "GenericAlgorithm.h"

class _305220980_A: public GenericAlgorithm {
	int counter;
	std::vector<Direction> possibleMoves;
public:

	 _305220980_A(): GenericAlgorithm(), counter(0){
		 possibleMoves = {Direction::North, Direction::West, Direction::South, Direction::East, Direction::Stay};
	 }

	 virtual Direction step(Direction prevStep) override;

	 virtual void setSensor(const AbstractSensor& sensor) override;
};

#endif /* 305220980A_H_ */
