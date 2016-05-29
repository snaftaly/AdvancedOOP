#ifndef _305220980_B_H_
#define _305220980_B_H_

#include "../Common/Direction.h"
#include "../Common/AbstractSensor.h"
#include "SmartGenericAlgorithm.h"

class _305220980_B: public SmartGenericAlgorithm {
	std::vector<Direction> possibleMoves;
public:

	 _305220980_B(): SmartGenericAlgorithm(){
		 possibleMoves = {Direction::North, Direction::South, Direction::West, Direction::East};
	 }

	 virtual Direction step(Direction prevStep) override;
};

#endif /* 305220980B_H_ */
