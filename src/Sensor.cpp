#include "Sensor.h"
#include "Direction.h"
// c'tor for the sensor
Sensor::Sensor(House * house, int i, int j) {
	sensorHouse = house;
	boti = i;
	botj = j;
}

SensorInformation Sensor::sense() const {
	struct SensorInformation sensorInfo;
	sensorInfo.dirtLevel = sensorHouse->getHouseMatrix()[boti][botj]; // dirt level
	sensorInfo.isWall[0] = sensorHouse->getHouseMatrix()[boti][botj+1] == 'W' ? true : false; // east
	sensorInfo.isWall[1] = sensorHouse->getHouseMatrix()[boti][botj-1] == 'W' ? true : false; // west
	sensorInfo.isWall[2] = sensorHouse->getHouseMatrix()[boti+1][botj] == 'W' ? true : false; // south
	sensorInfo.isWall[3] = sensorHouse->getHouseMatrix()[boti-1][botj] == 'W' ? true : false; // north
	return sensorInfo;
}

void Sensor::setBotLocation(int i, int j){
	boti = i;
	botj = j;
}
