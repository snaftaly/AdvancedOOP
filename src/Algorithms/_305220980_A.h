#ifndef _305220980_A_H_
#define _305220980_A_H_

#include "../Common/Direction.h"
#include "../Common/AbstractSensor.h"
#include "SmartGenericAlgorithm.h"

class _305220980_A: public SmartGenericAlgorithm {
	std::vector<Direction> possibleMoves;
public:

	 _305220980_A(): SmartGenericAlgorithm(){
		 possibleMoves = {Direction::West, Direction::East, Direction::North, Direction::South};
	 }

	 virtual Direction step(Direction prevStep) override;
};

#endif /* 305220980B_H_ */
