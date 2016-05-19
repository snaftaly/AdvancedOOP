#ifndef SMARTGENERICALGORITHM_H_
#define SMARTGENERICALGORITHM_H_

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
#include "../Common/Point.h"
#include "BatteryManager.h"


class SmartGenericAlgorithm: public AbstractAlgorithm {

	const AbstractSensor * sensor;
    struct CellInfo {
        int dirt = -1; // -1 represents "unknown"
        long stepsToDocking = -1; // -1 represents "unknown"
        bool isWall;
    };
    std::map<Point, CellInfo> houseMapping;
	BatteryManager batteryMng;

    long stepsFromDocking = -1;

	Direction prevStepFromAlgo = Direction::Stay; // = requestedStep in amir's implementation
    Direction currDir = Direction::East;
    Point currPosition = {0,0}; // {0,0} is the docking station position
    SensorInformation s;

	int stepsUntillFinishing = -1; // -1 tells it was not yet updated by the simulation using aboutToFinish()

	vector<Direction> directions;

    // informative information
	int maxStepsAfterWinner;

public:
	SmartGenericAlgorithm();
	virtual ~SmartGenericAlgorithm();

	virtual void setSensor(const AbstractSensor& sensor) override;
	virtual void setConfiguration(std::map<std::string, int> config) override;
	virtual Direction step(Direction prevStep) = 0;
	virtual void aboutToFinish(int stepsTillFinishing) override;
protected:
//	virtual void updatePreviousStep(const Direction & nextStep);
//	virtual void updateXYfromDock(const Direction & nextStep);
	virtual Direction getStepAndUpdatePrevStep(const std::vector<Direction>& possibleMoves, Direction stepFromSimulator, bool useCaution);
	bool isRobotInDock();

	// functions for smart algo:
	void updateAlgorithmInfo(Direction lastStep);
	int calcStepsToDocking(int stepsFromDocking, const Point& position);
	void updateStepsToDocking(int stepsToDocking, const Point& position);
	Direction getDirectionClosestToDocking();
	Direction  getDirectionToClosestNeededPlace();

};

#endif /* GENERICALGORITHM_H_ */
