/*
 * GenericAlgorithm.h
 *
 *  Created on: Apr 18, 2016
 *      Author: noaleibo1
 */

#ifndef GENERICALGORITHM_H_
#define GENERICALGORITHM_H_

#include <string>
#include <map>
#include <cstdlib>
#include <vector>
#include <stack>
#include "Sensor.h"
#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "BatteryManager.h"


class GenericAlgorithm: public AbstractAlgorithm {

	const AbstractSensor * sensor;
	int stepsUntillFinishing;
	int xDistanceFromDock;
	int yDistanceFromDock;
	std::stack<Direction> previousSteps;
	BatteryManager batteryMng;

public:
	GenericAlgorithm();
	virtual ~GenericAlgorithm();

	virtual void setSensor(const AbstractSensor& sensor) override;
	virtual void setConfiguration(std::map<std::string, int> config) override;
	virtual Direction step() = 0;
	virtual void aboutToFinish(int stepsTillFinishing) override;
protected:
	virtual Direction getStep(std::vector<Direction> possibleMoves);
	bool isRobotInDock();
};

#endif /* GENERICALGORITHM_H_ */
