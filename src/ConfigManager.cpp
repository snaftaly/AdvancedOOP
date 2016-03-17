#include "ConfigManager.h"
using namespace std;

// empty c'tor - puts default values in map
ConfigManager::ConfigManager() {
	confs.insert({"MaxSteps", 1200});
	confs.insert({"MaxStepsAfterWinner", 200});
	confs.insert({"BatteryCapacity", 400});
	confs.insert({"BatteryConsumptionRate", 1});
	confs.insert({"BatteryRachargeRate", 20});
}

// c'tor that loads values from file
ConfigManager::ConfigManager(std::string &path){
	// TODO: add code from tirgul 3
}

void ConfigManager::printConfs(){
	cout << "MaxSteps:" << confs["MaxSteps"] << endl;
	cout << "MaxStepsAfterWinner:" << confs["MaxStepsAfterWinner"] << endl;
	cout << "BatteryCapacity:" << confs["BatteryCapacity"] << endl;
	cout << "BatteryConsumptionRate:" << confs["BatteryConsumptionRate"] << endl;
	cout << "BatteryRachargeRate:" << confs["BatteryRachargeRate"] << endl;
}
