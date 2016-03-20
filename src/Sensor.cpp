#include "Sensor.h"
#include "Direction.h"

// empty c'tor for the sensor
Sensor::Sensor() {
	sensorHousePtr = NULL;
	robotiPrt = NULL;
	robotjPtr = NULL;
}

// c'tor for the sensor - TODO: might not be needed
//Sensor::Sensor(House * house, int * i, int * j) {
//	sensorHousePtr = house;
//	robotiPrt = i;
//	robotjPtr = j;
//}

SensorInformation Sensor::sense() const {
	struct SensorInformation sensorInfo;
	if (sensorHousePtr->getHouseMatrix()[robotiPrt][robotjPtr] == ' ' || sensorHousePtr->getHouseMatrix()[robotiPrt][robotjPtr] == 'D'){
		sensorInfo.dirtLevel = 0;
	}
	else {
		sensorInfo.dirtLevel = sensorHousePtr->getHouseMatrix()[robotiPrt][robotjPtr] - '0'; // dirt level
	}

	sensorInfo.isWall[(int)Direction::East] = sensorHousePtr->getHouseMatrix()[robotiPrt][robotjPtr+1] == 'W' ? true : false; // east
	sensorInfo.isWall[(int)Direction::West] = sensorHousePtr->getHouseMatrix()[robotiPrt][robotjPtr-1] == 'W' ? true : false; // west
	sensorInfo.isWall[(int)Direction::South] = sensorHousePtr->getHouseMatrix()[robotiPrt+1][robotjPtr] == 'W' ? true : false; // south
	sensorInfo.isWall[(int)Direction::North] = sensorHousePtr->getHouseMatrix()[robotiPrt-1][robotjPtr] == 'W' ? true : false; // north
	return sensorInfo;
}

void Sensor::setRobotLocation(int i, int j){
	robotiPrt = i;
	robotjPtr = j;
}

