#ifndef HOUSEMANAGER_H_
#define HOUSEMANAGER_H_
#include <string>
#include <vector>
#include <list>
#include "House.h"


// in the next exercises this class will get the houses from files and keep them in a list.
class HouseManager {

	std::list<House> houses;
	// private function - TODO: for later use
//	static bool hasHouseExtension(char * filename);
//	static void houseInput(House& house, std::string fileName);
//	void getHouses(std::string housesDir);
public:

	// HouseManager c'tor signature:
	HouseManager();

	// getters

	const std::list<House>& getHouses() const {
		return houses;
	}
};

#endif /* HOUSEMANAGER_H_ */
