#ifndef _305220980_B_H_
#define _305220980_B_H_

#include <string>
#include <map>
#include <cstdlib>
#include <vector>
#include <stack>
#include "Sensor.h"
#include "Direction.h"
#include "GenericAlgorithm.h"
#include "AbstractSensor.h"

class _305220980_B: public GenericAlgorithm {
	int counter;
	std::vector<Direction> possibleMoves;
public:

//	 GenericAlgorithm::GenericAlgorithm; // inherit ctor
	 _305220980_B(): GenericAlgorithm(), counter(0){
		 possibleMoves = {Direction::West, Direction::North, Direction::South, Direction::East, Direction::Stay};
	 }

	 virtual Direction step() override;
};

#endif /* 305220980B_H_ */
