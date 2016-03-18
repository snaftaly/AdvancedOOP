#include "Sensor.h"
#include "Direction.h"

// empty c'tor for the sensor
Sensor::Sensor() {
	sensorHouse = NULL;
	boti = 0;
	botj = 0;
}

// c'tor for the sensor - TODO: might not be needed
Sensor::Sensor(House * house, int i, int j) {
	sensorHouse = house;
	boti = i;
	botj = j;
}

SensorInformation Sensor::sense() const {
	struct SensorInformation sensorInfo;
	if (sensorHouse->getHouseMatrix()[boti][botj] == ' '){
		sensorInfo.dirtLevel = 0;
	}
	else {
		sensorInfo.dirtLevel = sensorHouse->getHouseMatrix()[boti][botj] - '0'; // dirt level
	}

	sensorInfo.isWall[(int)Direction::East] = sensorHouse->getHouseMatrix()[boti][botj+1] == 'W' ? true : false; // east
	sensorInfo.isWall[(int)Direction::West] = sensorHouse->getHouseMatrix()[boti][botj-1] == 'W' ? true : false; // west
	sensorInfo.isWall[(int)Direction::South] = sensorHouse->getHouseMatrix()[boti+1][botj] == 'W' ? true : false; // south
	sensorInfo.isWall[(int)Direction::North] = sensorHouse->getHouseMatrix()[boti-1][botj] == 'W' ? true : false; // north
	return sensorInfo;
}

void Sensor::setRobotLocation(int i, int j){
	boti = i;
	botj = j;
}

