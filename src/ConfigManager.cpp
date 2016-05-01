#include "ConfigManager.h"
#include "ErrorPrinter.h"
#include "FileUtils.h"
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <string>

using namespace std;


bool ConfigManager::loadFromFile()
{
	// loads the confs to the map and returns true if there was no failure reading from the file.
	confs.clear();
	string confFilePath = confPath + "config.ini";

	// first check if file exists, otherwise print usage
	if (( access( confFilePath.c_str(), F_OK ) == -1 )){
		// TODO: should it be the path of the folder or path to the file iteslf?
		ErrorPrinter::printUsage();
		cout << "cannot find config.ini file in '" << FileUtils::getFullPath(confPath) << "'" << endl;
		return false;
	}

	ifstream fin(confFilePath);
	if (!fin.is_open()){ // error opening file
		cout << "config.ini exists in '"<< FileUtils::getFullPath(confFilePath) << "' but cannot be opened" << endl;
		return false;
	}
	string line;
	while (getline(fin, line))
	{
		processLine(line);
	}
	fin.close();
	// TODO: what should be the precedence here?
	// now check that the the parameters have correct values and missing paramas
	updateParamsWithBaValue();
	updateMissingParams();

	if (!missingParams.empty() || !paramsWithBadValues.empty()){
		printParamsWithBadValues();
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
	int tempVal;
	if (tokens.size() < 1 || tokens.size() > 2)
	{
		// TODO: what should be done in case the value is empty?
		return;
	}
	string key = trim(tokens[0]);
	if (!key.compare("MaxStepsAfterWinner") || !key.compare("BatteryCapacity") || !key.compare("BatteryConsumptionRate") ||
			!key.compare("BatteryRechargeRate")){
		if (tokens.size() == 1){
			tempVal =  -1;
		}
		else {
			try{
				tempVal = stoi(tokens[1]);
			}
			catch (...){
				tempVal = -1; // there was an error converting to number, -1 indicates error
			}
		}
		confs[key] = tempVal;
	}

}

void ConfigManager::printConfs()
{
	for(const pair<string, int>& confValPair : confs) {
		cout << confValPair.first << ": " << confValPair.second << endl;
	}
}

void ConfigManager::updateMissingParams(){
	// check if there are missing confs, and if so add to list of missing params and return true
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
}

void ConfigManager::printMissingParams()
{
	if (missingParams.empty()) return;
	cout << "config.ini missing " << missingParams.size() << " parameter(s): ";
	for (auto iter = missingParams.begin(); iter != missingParams.end(); ++iter) {
		if (iter != missingParams.begin()) cout << ", ";
		cout << *iter;
	}
	cout << endl;
}

void ConfigManager::updateParamsWithBaValue(){
	// check if there are bad values for params, and if so put in mi
	for(const pair<string, int>& confValPair : confs) {
		if (confValPair.second < 0){
			// value of param is negative indicating an error
			paramsWithBadValues.push_back(confValPair.first);
		}
	}
}


void ConfigManager::printParamsWithBadValues()
{
	if (paramsWithBadValues.empty()) return;
	cout << "config.ini having bad values for " << paramsWithBadValues.size() << " parameter(s): ";
	for (auto iter = paramsWithBadValues.begin(); iter != paramsWithBadValues.end(); ++iter) {
		if (iter != paramsWithBadValues.begin()) cout << ", ";
		cout << *iter;
	}
	cout << endl;
}
