#include "ConfigManager.h"
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

// ConfigManager c'tor that initializes by default values
ConfigManager::ConfigManager()
{
	MaxSteps = 1200;
	MaxStepsAfterWinner = 200;
	BatteryCapacity = 400;
	BatteryConsumptionRate = 1;
	BatteryRachargeRate = 20;
}

// ConfigManager c'tor that initializes by con file
ConfigManager::ConfigManager(std::string &path)
{
	std::string tempLine;
	std::string delim = "=";
	std::string key;
	int delimInd;
	int val;

	std::ifstream configFileStream(path.append("/config.ini").c_str());
	for (int i = 0; i < 5; i++){ // go over each line of the file
		std::getline(configFileStream, tempLine);
		cout << tempLine << endl;
		delimInd = tempLine.find(delim);
		key = tempLine.substr(0, delimInd);
		val = atoi(tempLine.substr(delimInd + 1 , tempLine.size()).c_str());
		if (key.compare("MaxSteps") == 0){
			MaxSteps = val;
		}
		else if (key.compare("MaxStepsAfterWinner") == 0){
			MaxStepsAfterWinner=val;
		}
		else if (key.compare("BatteryCapacity") == 0){
			BatteryCapacity = val;
		}
		else if (key.compare("BatteryConsumptionRate") == 0){
			BatteryConsumptionRate = val;
		}
		else if (key.compare("BatteryRachargeRate") == 0){
			BatteryRachargeRate = val;
		}
	}
}

void ConfigManager::printConf(){
	cout << "MaxSteps:" << MaxSteps << endl;
	cout << "MaxStepsAfterWinner:" << MaxStepsAfterWinner << endl;
	cout << "BatteryCapacity:" << BatteryCapacity << endl;
	cout << "BatteryConsumptionRate:" << BatteryConsumptionRate << endl;
	cout << "BatteryRachargeRate:" << BatteryRachargeRate << endl;
}

