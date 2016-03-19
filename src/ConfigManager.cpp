#include "ConfigManager.h"
#include <fstream>
#include <sstream>
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
ConfigManager::ConfigManager(const string& path){
	loadFromFile(path);
}

void ConfigManager::loadFromFile(const string& iniPath)
 {
   confs.clear();
   ifstream fin(iniPath.c_str());
   string line;
   while (getline(fin, line))
   {
     processLine(line);
   }
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

void ConfigManager::printConfs(){
	cout << "MaxSteps:" << confs["MaxSteps"] << endl;
	cout << "MaxStepsAfterWinner:" << confs["MaxStepsAfterWinner"] << endl;
	cout << "BatteryCapacity:" << confs["BatteryCapacity"] << endl;
	cout << "BatteryConsumptionRate:" << confs["BatteryConsumptionRate"] << endl;
	cout << "BatteryRachargeRate:" << confs["BatteryRachargeRate"] << endl;
}
