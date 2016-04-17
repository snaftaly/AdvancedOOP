#ifndef SIMPLEALGORITHM_H_
#define SIMPLEALGORITHM_H_

#include <string>
#include <map>
#include <cstdlib>
#include <vector>
#include "Sensor.h"
#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "AlgosCommon.h"

class SimpleAlgorithm: public AbstractAlgorithm {

	// need to use AbstractSensor *
	const AbstractSensor * sensor;
	std::map<std::string, int> configs;

public:

	SimpleAlgorithm();
	~SimpleAlgorithm();

	void setSensor(const AbstractSensor& sensor);
	void setConfiguration(std::map<std::string, int> config);
	Direction step();
	void aboutToFinish(int stepsTillFinishing);
	std::vector<Direction> getPossibleSteps();

};

#endif /* SIMPLEALGORITHM_H_ */
