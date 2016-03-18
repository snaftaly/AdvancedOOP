#ifndef SENSOR_H_
#define SENSOR_H_
#include "AbstractSensor.h"
#include "House.h"

class Sensor: public AbstractSensor {
	// private
	House * sensorHouse;
	int boti, botj;
public:
	// sensor c'tor declarations
	Sensor();
	Sensor(House * house, int i, int j); // TODO: might not be needed
	// empty d'tor
	~Sensor() {};

	SensorInformation sense() const;
	void setRobotLocation(int i, int j);

	// getters

	// setters
	void setSensorHouse(House * sensorHouse) {
		this->sensorHouse = sensorHouse;
	}
};

#endif /* SENSOR_H_ */
