#ifndef CONFIGMANAGER_H_
#define CONFIGMANAGER_H_
#include <iostream>
#include <string>
#include <map>
#include <vector>

class ConfigManager {
	std::map<std::string, int> confs;
	void processLine(const std::string& line);
	static std::string trim(std::string& str);
	static std::vector<std::string> split(const std::string &s, char delim);

public:
	ConfigManager(bool useDefault);
	bool loadFromFile(const std::string& iniPath);
	void printConfs();

	// getters
	const std::map<std::string, int>& getConfs() const {
		return confs;
	}
};

#endif /* CONFIGMANAGER_H_ */
