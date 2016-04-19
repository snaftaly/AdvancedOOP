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

class _305220980_B: public AbstractAlgorithm {

	const AbstractSensor * sensor;
	std::map<std::string, int> configs;
	std::stack<Direction> previousSteps;
	int stepsUntillFinishing;

public:

	_305220980_B();
	~_305220980_B();

	void setSensor(const AbstractSensor& sensor);
	void setConfiguration(std::map<std::string, int> config);
	Direction step();
	void aboutToFinish(int stepsTillFinishing);
	Direction getStep();
};

#endif /* 305220980B_H_ */
