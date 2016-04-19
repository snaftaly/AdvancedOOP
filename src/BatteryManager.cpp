#include "BatteryManager.h"
using namespace std;

BatteryManager::BatteryManager(int _batteryCapacity, int _batteryRechargeRate, int _batteryRechargeRate) {
	batteryState = 0;
	BatteryCapacity = _batteryCapacity;
	batteryConsumptionRate = _batteryRechargeRate;
	batteryRechargeRate = _batteryRechargeRate;
	}

BatteryManager::~BatteryManager() { }

void BatteryManager::chargeBattery(){
	batteryState = max(batteryState + batteryRechargeRate, BatteryCapacity);
}

void BatteryManager::consumeBattery(){
	batteryState -= min(batteryState - batteryConsumptionRate, 0);
}

bool BatteryManager::isBatteryFull(){
	return batteryState >= BatteryCapacity;
}
