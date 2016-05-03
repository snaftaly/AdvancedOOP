#include "Sensor.h"
#include "../Common/Direction.h"
#include <iostream>
using namespace std;

// empty c'tor for the sensor
Sensor::Sensor() {
	sensorHousePtr = NULL;
	robotiPtr = NULL;
	robotjPtr = NULL;
}

SensorInformation Sensor::sense() const{
	struct SensorInformation sensorInfo;
	char currPlace = sensorHousePtr->getHouseMatrix()[*robotiPtr][*robotjPtr];

	if (currPlace >= '0' && currPlace <='9'){
		sensorInfo.dirtLevel = sensorHousePtr->getHouseMatrix()[*robotiPtr][*robotjPtr] - '0'; // dirt level
	}
	else { // currPlace == ' ' or 'W' or any other char that is not '0'-'9' or 'W'
		sensorInfo.dirtLevel = 0;
	}

	sensorInfo.isWall[(int)Direction::East] = sensorHousePtr->getHouseMatrix()[*robotiPtr][*robotjPtr+1] == 'W' ? true : false;
	sensorInfo.isWall[(int)Direction::West] = sensorHousePtr->getHouseMatrix()[*robotiPtr][*robotjPtr-1] == 'W' ? true : false;
	sensorInfo.isWall[(int)Direction::South] = sensorHousePtr->getHouseMatrix()[*robotiPtr+1][*robotjPtr] == 'W' ? true : false;
	sensorInfo.isWall[(int)Direction::North] = sensorHousePtr->getHouseMatrix()[*robotiPtr-1][*robotjPtr] == 'W' ? true : false;
	return sensorInfo;
}


