#ifndef _305220980_B_H_
#define _305220980_B_H_

#include "Sensor.h"
#include "Direction.h"
#include "GenericAlgorithm.h"
#include "AbstractSensor.h"

class _305220980_B: public GenericAlgorithm {
	int counter;
	std::vector<Direction> possibleMoves;
public:

	 _305220980_B(): GenericAlgorithm(), counter(0){
		 possibleMoves = {Direction::East};
	 }

	 virtual Direction step(Direction prevStep) override;
};

#endif /* 305220980B_H_ */
