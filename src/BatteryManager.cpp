/*
 * BatteryManager.cpp
 *
 *  Created on: Apr 18, 2016
 *      Author: noaleibo1
 */

#include "BatteryManager.h"

BatteryManager::BatteryManager(): batteryState(0) {
	// TODO Auto-generated constructor stub

}

BatteryManager::~BatteryManager() {
	// TODO Auto-generated destructor stub
}

void BatteryManager::setConfiguration(std::map<std::string, int> config){//TODO remove?
	configs = config;
}
void BatteryManager::updateBatteryState(int addToBattery){
	batteryState += addToBattery;
}
