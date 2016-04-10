#ifndef HOUSEMANAGER_H_
#define HOUSEMANAGER_H_
#include <string>
#include <list>
#include <map>
#include "House.h"


// in the next exercises this class will get the houses from files and keep them in a list.
class HouseManager {
	std::string housesPath;
	int numValidHouses;
	std::list<std::string> housesFileNamesLst;
	std::list<House> houses;
	std::map<std::string, std::string> housesErrors;
public:

	// HouseManager c'tor signature:
	HouseManager(const std::string& housesPath): housesPath(housesPath), numValidHouses(0){ }
	bool readHousesFiles();
	void printHousesErrors(bool all);

	// getters

	const std::list<House>& getHouses() {
		return houses;
	}

	int getNumValidHouses() const {
		return numValidHouses;
	}
};

#endif /* HOUSEMANAGER_H_ */
