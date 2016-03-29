#include "ConfigManager.h"
#include <fstream>
#include <sstream>
using namespace std;

// empty c'tor - puts default values in map
ConfigManager::ConfigManager(bool useDefault) {
	if (useDefault){
		confs.insert({"MaxSteps", 1200});
		confs.insert({"MaxStepsAfterWinner", 200});
		confs.insert({"BatteryCapacity", 400});
		confs.insert({"BatteryConsumptionRate", 1});
		confs.insert({"BatteryRechargeRate", 20});
	}
}

bool ConfigManager::loadFromFile(const string& iniPath)
{
	// loads the confs to the map and returns true if there was no failure reading from the file.
	confs.clear();
	string confFileFullPath = iniPath+"/config.ini";
	ifstream fin(confFileFullPath.c_str());
	if (!fin.is_open()){
		cout << "Error opening configuration file. Quitting." << endl;
		return false;
	}
	string line;
	while (getline(fin, line))
	{
		processLine(line);
	}
	if (fin.bad()){
		cout << "Error while reading file " << iniPath << "/config.ini" << endl;
		fin.close();
		return false;
	}
	fin.close();
	// reading of file succeeded, now check that alll values are in the dict
	checkParameters();
	return true;
}

vector<std::string> ConfigManager::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

string ConfigManager::trim(std::string& str)
{
	str.erase(0, str.find_first_not_of(' '));       //prefixing spaces
	str.erase(str.find_last_not_of(' ')+1);         //surfixing spaces
	return str;
}

void ConfigManager::processLine(const string& line)
{
	vector<string> tokens = split(line, '=');
	if (tokens.size() != 2)
	{
		return;
	}
	confs[trim(tokens[0])] = stoi(tokens[1]);
}

void ConfigManager::printConfs()
{
	cout << "MaxSteps:" << confs["MaxSteps"] << endl;
	cout << "MaxStepsAfterWinner:" << confs["MaxStepsAfterWinner"] << endl;
	cout << "BatteryCapacity:" << confs["BatteryCapacity"] << endl;
	cout << "BatteryConsumptionRate:" << confs["BatteryConsumptionRate"] << endl;
	cout << "BatteryRechargeRate:" << confs["BatteryRechargeRate"] << endl;
}

void ConfigManager::checkParameters(){
	// check if the conf is found in confs and if not add default value
	if ( confs.find("MaxSteps") == confs.end() ){
		confs.insert({"MaxSteps", 1200});
	}
	if ( confs.find("MaxStepsAfterWinner") == confs.end() ){
		confs.insert({"MaxStepsAfterWinner", 200});
	}
	if ( confs.find("BatteryCapacity") == confs.end() ){
		confs.insert({"BatteryCapacity", 400});
	}
	if ( confs.find("BatteryConsumptionRate") == confs.end() ){
		confs.insert({"BatteryConsumptionRate", 1});
	}
	if ( confs.find("BatteryRechargeRate") == confs.end() ){
		confs.insert({"BatteryRechargeRate", 20});
	}
}

