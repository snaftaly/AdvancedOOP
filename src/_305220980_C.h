#ifndef _305220980_C_H_
#define _305220980_C_H_

#include "Sensor.h"
#include "Direction.h"
#include "GenericAlgorithm.h"
#include "AbstractSensor.h"

class _305220980_C: public GenericAlgorithm {
	std::vector<Direction> possibleMoves;
public:

	 _305220980_C(): GenericAlgorithm(){
		 possibleMoves = {Direction::East, Direction::Stay};
	 }

	 virtual Direction step() override;
};

#endif /* 305220980B_H_ */
