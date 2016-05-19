#ifndef _305220980_D_H_
#define _305220980_D_H_

#include <map>
#include "../Common/Direction.h"
#include "../Common/AbstractSensor.h"
#include "../Common/Point.h"

class _305220980_D : public AbstractAlgorithm {
    const AbstractSensor* sensor;
    struct CellInfo {
        int dirt = -1; // -1 represents "unknown"
        int stepsToDocking = -1; // -1 represents "unknown"
        bool isWall;
    };
    std::map<Point, CellInfo> houseMapping;
    Direction currDir = Direction::East;
    Point position = {0,0};
    SensorInformation s;
    int stepsFromDocking = -1;
    int totalSteps = 0;
    int phase = -1; // phases 0-7: explore surrounding, phase 8: select new direction
    const static Direction dir_by_phase[8];
    Direction requestedStep = Direction::Stay;

	int stepsUntillFinishing;
	Direction prevStepFromAlgo;
	bool isGoingBack;
	std::stack<Direction> previousSteps;
	BatteryManager batteryMng;
	int maxStepsAfterWinner;

public:
	_305220980_D(): sensor(NULL){};

	 virtual Direction step(Direction prevStep) override;

	 virtual void setSensor(const AbstractSensor& sensor) override;
};

#endif /* _305220980D_H_ */
