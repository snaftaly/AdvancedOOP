#ifndef CONFIGMANAGER_H_
#define CONFIGMANAGER_H_
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <vector>

class ConfigManager {
	std::string confPath;
	std::map<std::string, int> confs;
	std::list<std::string> paramsWithBadValues;
	std::list<std::string> missingParams;

	void processLine(const std::string& line);
	static std::string trim(std::string& str);
	static std::vector<std::string> split(const std::string &s, char delim);
	void updateMissingParams();
	void printMissingParams();
	void updateParamsWithBaValue();
	void printParamsWithBadValues();

public:
	// c'tor for conf manager
	ConfigManager(std::string _confPath): confPath(_confPath){ };

	bool loadFromFile();
	void printConfs();

	// getters
	const std::map<std::string, int>& getConfs() const {
		return confs;
	}
};

#endif /* CONFIGMANAGER_H_ */
