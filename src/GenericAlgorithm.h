#ifndef GENERICALGORITHM_H_
#define GENERICALGORITHM_H_

#include <string>
#include <map>
#include <cstdlib>
#include <vector>
#include <stack>
#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AbstractSensor.h"
#include "BatteryManager.h"
#include "AlgosCommon.h"


class GenericAlgorithm: public AbstractAlgorithm {

	const AbstractSensor * sensor;
	int stepsUntillFinishing;
	int xDistanceFromDock;
	int yDistanceFromDock;
	std::stack<Direction> previousSteps;
	BatteryManager batteryMng;
	int maxStepsAfterWinner;

public:
	GenericAlgorithm();
	virtual ~GenericAlgorithm();

	virtual void setSensor(const AbstractSensor& sensor) override;
	virtual void setConfiguration(std::map<std::string, int> config) override;
	virtual Direction step() = 0;
	virtual void aboutToFinish(int stepsTillFinishing) override;
protected:
	virtual void updatePreviousStep(const Direction & nextStep);
	virtual void updateXYfromDock(const Direction & nextStep);
	virtual Direction getStep(const std::vector<Direction>& possibleMoves);
	bool isRobotInDock();
};

#endif /* GENERICALGORITHM_H_ */
