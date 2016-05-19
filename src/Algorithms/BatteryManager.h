#ifndef BATTERYMANAGER_H_
#define BATTERYMANAGER_H_

class BatteryManager {

	int batteryState;

	// informative information
	int batteryCapacity;
	int batteryConsumptionRate;
	int batteryRechargeRate;

public:
	BatteryManager():
		batteryState(0), batteryCapacity(0), batteryConsumptionRate(0), batteryRechargeRate(0){}
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
		return batteryCapacity;
	}

	int getBatteryConsumptionRate() const {
		return batteryConsumptionRate;
	}

	int getBatteryRechargeRate() const {
		return batteryRechargeRate;
	}

	void setBatteryCapacity(int batteryCapacity) {
		this->batteryCapacity = batteryCapacity;
	}

	void setBatteryConsumptionRate(int batteryConsumptionRate) {
		this->batteryConsumptionRate = batteryConsumptionRate;
	}

	void setBatteryRechargeRate(int batteryRechargeRate) {
		this->batteryRechargeRate = batteryRechargeRate;
	}
};

#endif /* BATTERYMANAGER_H_ */
