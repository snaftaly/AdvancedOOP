#ifndef SENSOR_H_
#define SENSOR_H_
#include "AbstractSensor.h"
#include "House.h"

class Sensor: public AbstractSensor {
	// private
	House * sensorHouse;
	int boti, botj;
public:
	// sensor c'tor declaration
	Sensor(House * house, int i, int j);
	// empty d'tor
	~Sensor() {};

	SensorInformation sense() const;
	void setBotLocation(int i, int j);

};

#endif /* SENSOR_H_ */
