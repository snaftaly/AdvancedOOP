#ifndef _305220980_C_H_
#define _305220980_C_H_

#include "../Common/Direction.h"
#include "../Common/AbstractSensor.h"
#include "SmartGenericAlgorithm.h"

class _305220980_C: public SmartGenericAlgorithm {
	std::vector<Direction> possibleMoves;
public:

	 _305220980_C(): SmartGenericAlgorithm(){
		 possibleMoves = {Direction::North, Direction::West, Direction::East, Direction::South};
	 }

	 virtual Direction step(Direction prevStep) override;
};

#endif /* 305220980C_H_ */
