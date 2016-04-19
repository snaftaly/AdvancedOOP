#ifndef BATTERYMANAGER_H_
#define BATTERYMANAGER_H_

class BatteryManager {

	int batteryState;
	int BatteryCapacity;
	int batteryConsumptionRate;
	int batteryRechargeRate;

public:
	BatteryManager(int _batteryCapacity, int _batteryRechargeRate, int _batteryRechargeRate);
	virtual ~BatteryManager();

	void chargeBattery();
	void consumeBattery();
	void resetBattery();
	bool isBatteryFull();

	// getters

	int getBatteryState() const {
		return batteryState;
	}

	int getBatteryCapacity() const {
		return BatteryCapacity;
	}

	int getBatteryConsumptionRate() const {
		return batteryConsumptionRate;
	}

	int getBatteryRechargeRate() const {
		return batteryRechargeRate;
	}
};

#endif /* BATTERYMANAGER_H_ */
