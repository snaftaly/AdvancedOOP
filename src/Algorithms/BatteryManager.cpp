#include "BatteryManager.h"
#include <algorithm>
#include <iostream>
using namespace std;

BatteryManager::~BatteryManager() { }

void BatteryManager::chargeBattery(){
	batteryState = std::min(batteryState + batteryRechargeRate, batteryCapacity);
}

void BatteryManager::consumeBattery(){
	batteryState = std::max(batteryState - batteryConsumptionRate, 0);
}

bool BatteryManager::isBatteryFull(){
	return batteryState >= batteryCapacity;
}

void BatteryManager::resetBattery(){
	batteryState = batteryCapacity;
}
