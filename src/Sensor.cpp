#include "Sensor.h"
#include "Direction.h"

// empty c'tor for the sensor
Sensor::Sensor() {
	sensorHouse = NULL;
	roboti = 0;
	robotj = 0;
}

// c'tor for the sensor - TODO: might not be needed
Sensor::Sensor(House * house, int i, int j) {
	sensorHouse = house;
	roboti = i;
	robotj = j;
}

SensorInformation Sensor::sense() const {
	struct SensorInformation sensorInfo;
	if (sensorHouse->getHouseMatrix()[roboti][robotj] == ' ' || sensorHouse->getHouseMatrix()[roboti][robotj] == 'D'){
		sensorInfo.dirtLevel = 0;
	}
	else {
		sensorInfo.dirtLevel = sensorHouse->getHouseMatrix()[roboti][robotj] - '0'; // dirt level
	}

	sensorInfo.isWall[(int)Direction::East] = sensorHouse->getHouseMatrix()[roboti][robotj+1] == 'W' ? true : false; // east
	sensorInfo.isWall[(int)Direction::West] = sensorHouse->getHouseMatrix()[roboti][robotj-1] == 'W' ? true : false; // west
	sensorInfo.isWall[(int)Direction::South] = sensorHouse->getHouseMatrix()[roboti+1][robotj] == 'W' ? true : false; // south
	sensorInfo.isWall[(int)Direction::North] = sensorHouse->getHouseMatrix()[roboti-1][robotj] == 'W' ? true : false; // north
	return sensorInfo;
}

void Sensor::setRobotLocation(int i, int j){
	roboti = i;
	robotj = j;
}

