#ifndef HOUSEMANAGER_H_
#define HOUSEMANAGER_H_
#include <string>
#include <vector>
#include <list>
#include "House.h"


// in the next exercises this class will get the houses from files and keep them in a list.
class HouseManager {
	int numValidHouses;
	std::list<House> houses;
public:

	// HouseManager c'tor signature:
	HouseManager(const std::string& housesPath);

	// getters

	const std::list<House>& getHouses() {
		return houses;
	}

	int getNumValidHouses() const {
		return numValidHouses;
	}
};

#endif /* HOUSEMANAGER_H_ */
