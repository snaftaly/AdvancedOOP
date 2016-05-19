#ifndef HOUSEMANAGER_H_
#define HOUSEMANAGER_H_
#include <string>
#include <list>
#include <vector>
#include <map>
#include "House.h"


// in the next exercises this class will get the houses from files and keep them in a list.
class HouseManager {
	std::string housesPath;
	std::vector<std::string> housesFileNames;
	std::list<std::string> validHousesFileNames;
	std::map<std::string, std::string> housesErrors;
public:
	// HouseManager c'tor:
	HouseManager(const std::string& _housesPath): housesPath(_housesPath){ }
	bool readHousesFiles();
	void printHousesErrors(bool all);

	void addHouseErr(const std::string& houseeFileName, const std::string& errorStr);
	void addValidHouse(const std::string& houseeFileName);

	// getters

	const std::map<std::string, std::string>& getHousesErrors() const {
		return housesErrors;
	}

	const std::string& getHousesPath() const {
		return housesPath;
	}

	const std::vector<std::string>& getHousesFileNames() const {
		return housesFileNames;
	}

	std::list<std::string>& getValidHousesFileNamesSorted();
};

#endif /* HOUSEMANAGER_H_ */
