#ifndef CONFIGMANAGER_H_
#define CONFIGMANAGER_H_

#include <iostream>
#include <string>
#include <map>

class ConfigManager {
	std::map<std::string, int> confs;
public:
	ConfigManager();
	ConfigManager(std::string &path);
	void printConfs();

	// getters
	const std::map<std::string, int>& getConfs() const {
		return confs;
	}
};

#endif /* CONFIGMANAGER_H_ */
