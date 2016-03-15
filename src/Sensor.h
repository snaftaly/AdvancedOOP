#ifndef SENSOR_H_
#define SENSOR_H_
#include "AbstractSensor.h"
#include "House.h"

class Sensor: public AbstractSensor {
	House * sensorHouse;
	int boti, botj;
public:
	Sensor(House * house, int i, int j);
	virtual ~Sensor();

    // sense function declaration
	SensorInformation sense() const;
};

#endif /* SENSOR_H_ */
