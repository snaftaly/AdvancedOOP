/*
 * BatteryManager.h
 *
 *  Created on: Apr 18, 2016
 *      Author: noaleibo1
 */

#ifndef BATTERYMANAGER_H_
#define BATTERYMANAGER_H_

#include <map>
#include <string>

class BatteryManager {

	std::map<std::string, int> configs;
	int batteryState;

public:
	BatteryManager();
	virtual ~BatteryManager();

	void setConfiguration(std::map<std::string, int> config);
	void updateBatteryState(int addToBattery);

	int getBatteryState() const {
		return batteryState;
	}

	void setBatteryState(int batteryState) {
		this->batteryState = batteryState;
	}
};

#endif /* BATTERYMANAGER_H_ */
