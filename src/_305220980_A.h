#ifndef _305220980_A_H_
#define _305220980_A_H_

#include <string>
#include <map>
#include <cstdlib>
#include <vector>
#include <stack>
#include "Sensor.h"
#include "Direction.h"
#include "GenericAlgorithm.h"
#include "AbstractSensor.h"

class _305220980_A: public GenericAlgorithm {

public:

	using GenericAlgorithm::GenericAlgorithm; // inherit ctor

	virtual Direction step() override;
};

#endif /* 305220980A_H_ */
