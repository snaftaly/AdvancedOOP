#include "Sensor.h"
#include "../Common/Direction.h"
#include <iostream>
using namespace std;

// empty c'tor for the sensor
Sensor::Sensor() {
	sensorHousePtr = NULL;
	robotPosPtr = NULL;
//	robotjPtr = NULL;
}

SensorInformation Sensor::sense() const{
	struct SensorInformation sensorInfo;
	char currPlace = sensorHousePtr->getHouseMatrix()[robotPosPtr->getY()][robotPosPtr->getX()];

	if (currPlace >= '0' && currPlace <='9'){
		sensorInfo.dirtLevel = sensorHousePtr->getHouseMatrix()[robotPosPtr->getY()][robotPosPtr->getX()] - '0'; // dirt level
	}
	else { // currPlace == ' ' or 'W' or any other char that is not '0'-'9' or 'W'
		sensorInfo.dirtLevel = 0;
	}

	sensorInfo.isWall[(int)Direction::East] = sensorHousePtr->getHouseMatrix()[robotPosPtr->getY()][robotPosPtr->getX()+1] == 'W' ? true : false;
	sensorInfo.isWall[(int)Direction::West] = sensorHousePtr->getHouseMatrix()[robotPosPtr->getY()][robotPosPtr->getX()-1] == 'W' ? true : false;
	sensorInfo.isWall[(int)Direction::South] = sensorHousePtr->getHouseMatrix()[robotPosPtr->getY()+1][robotPosPtr->getX()] == 'W' ? true : false;
	sensorInfo.isWall[(int)Direction::North] = sensorHousePtr->getHouseMatrix()[robotPosPtr->getY()-1][robotPosPtr->getX()] == 'W' ? true : false;
	return sensorInfo;
}


