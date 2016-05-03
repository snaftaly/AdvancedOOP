#ifndef GENERICALGORITHM_H_
#define GENERICALGORITHM_H_

#include <string>
#include <map>
#include <cstdlib>
#include <vector>
#include <stack>
#include "../Common/Direction.h"
#include "../Common/AbstractAlgorithm.h"
#include "../Common/AbstractSensor.h"
#include "../Common/MakeUniqueAdder.h"
#include "../Common/AlgorithmRegistration.h"
#include "BatteryManager.h"


class GenericAlgorithm: public AbstractAlgorithm {

	const AbstractSensor * sensor;
	int stepsUntillFinishing;
	int xDistanceFromDock;
	int yDistanceFromDock;
	Direction prevStepFromAlgo;
	bool isGoingBack;
	std::stack<Direction> previousSteps;
	BatteryManager batteryMng;
	int maxStepsAfterWinner;

public:
	GenericAlgorithm();
	virtual ~GenericAlgorithm();

	virtual void setSensor(const AbstractSensor& sensor) override;
	virtual void setConfiguration(std::map<std::string, int> config) override;
	virtual Direction step(Direction prevStep) = 0;
	virtual void aboutToFinish(int stepsTillFinishing) override;
protected:
	virtual void updatePreviousStep(const Direction & nextStep);
	virtual void updateXYfromDock(const Direction & nextStep);
	virtual Direction getStepAndUpdatePrevStep(const std::vector<Direction>& possibleMoves, Direction stepFromSimulator);
	bool isRobotInDock();
};

#endif /* GENERICALGORITHM_H_ */
