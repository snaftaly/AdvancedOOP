#include "ConfigManager.h"
#include "ErrorPrinter.h"
//#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>

using namespace std;


bool ConfigManager::loadFromFile()
{
	// loads the confs to the map and returns true if there was no failure reading from the file.
	confs.clear();
	string confFileFullPath = confPath + "config.ini";

	// first check if file exists, otherwise print usage
	if (( access( confFileFullPath.c_str(), F_OK ) == -1 )){
		ErrorPrinter::printUsage();
		return false;
	}

	ifstream fin(confFileFullPath);
	if (!fin.is_open()){ // error opening file // TODO: print full path
		cout << "config.ini exists in '"<< confFileFullPath << "' but cannot be opened" << endl;
		return false;
	}
	string line;
	while (getline(fin, line))
	{
		processLine(line);
	}
	fin.close();
	// now check that all values are in the dict
	if (isMissingParams())
	{
		printMissingParams();
		return false;
	}
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
	for(const pair<string, int>& confValPair : confs) {
		cout << confValPair.first << ": " << confValPair.second << endl;
	}
}

bool ConfigManager::isMissingParams(){
	// check if there are missing confs, and if so print error
	if ( confs.find("MaxStepsAfterWinner") == confs.end() ){
		missingParams.emplace_back("MaxStepsAfterWinner");
	}
	if ( confs.find("BatteryCapacity") == confs.end() ){
		missingParams.emplace_back("BatteryCapacity");
	}
	if ( confs.find("BatteryConsumptionRate") == confs.end() ){
		missingParams.emplace_back("BatteryConsumptionRate");
	}
	if ( confs.find("BatteryRechargeRate") == confs.end() ){
		missingParams.emplace_back("BatteryRechargeRate");
	}
	if (!missingParams.empty()){
		return true;
	}
	return false;
}

void ConfigManager::printMissingParams()
{
	cout << "config.ini missing " << missingParams.size() << " parameter(s): ";
	for (auto iter = missingParams.begin(); iter != missingParams.end(); ++iter) {
		if (iter != missingParams.begin()) cout << ", ";
		cout << *iter;
	}
	cout << endl;
}
