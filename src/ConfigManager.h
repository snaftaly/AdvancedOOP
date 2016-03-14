#ifndef CONFIGMANAGER_H_
#define CONFIGMANAGER_H_

#include <string>

class ConfigManager {
public:
	// data members
	int MaxSteps;
	int MaxStepsAfterWinner;
	int BatteryCapacity;
	int BatteryConsumptionRate;
	int BatteryRachargeRate;

	// functions signatures

	// ConfigManager c'tor signatures
	ConfigManager(); // default c'tor with default values
	ConfigManager(std::string &path);

	void printConf();
};

#endif /* CONFIGMANAGER_H_ */
