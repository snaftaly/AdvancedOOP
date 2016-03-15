#include "Sensor.h"

// c'tor for the sensor
Sensor::Sensor(House * house, int i, int j) {
	sensorHouse = house;
	boti = i;
	botj = j;
}

Sensor::~Sensor() {
	// TODO What should be done here?
}

SensorInformation Sensor::sense() const {
	struct SensorInformation sensorInfo = new struct SensorInformation;
	sensorInfo.dirtLevel = sensorHouse->houseMatrix[boti][botj]; // dirt level
	sensorInfo.isWall[0] = sensorHouse->houseMatrix[boti][botj+1] == 'W' ? true : false; // East
	sensorInfo.isWall[1] = sensorHouse->houseMatrix[boti][botj-1] == 'W' ? true : false; // West
	sensorInfo.isWall[2] = sensorHouse->houseMatrix[boti-1][botj] == 'W' ? true : false; // South
	sensorInfo.isWall[3] = sensorHouse->houseMatrix[boti+1][botj] == 'W' ? true : false; // North
	return sensorInfo;
}
