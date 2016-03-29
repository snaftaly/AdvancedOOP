#ifndef SENSOR_H_
#define SENSOR_H_
#include "AbstractSensor.h"
#include "House.h"
#include <iostream>

class Sensor: public AbstractSensor {
	// private
	House * sensorHousePtr;
	int * robotiPtr, * robotjPtr;
public:
	// sensor c'tor declarations
	Sensor();
	// empty d'tor
	~Sensor() {};

	SensorInformation sense() const;
	void setRobotLocation(int i, int j);

	// setters
	void setSensorHouse(House * sensorHouse) {
		this->sensorHousePtr = sensorHouse;
	}

	void setRobotiPrt(int* robotiPrt) {
		this->robotiPtr = robotiPrt;
	}

	void setRobotjPtr(int* robotjPtr) {
		this->robotjPtr = robotjPtr;
	}
};

#endif /* SENSOR_H_ */
