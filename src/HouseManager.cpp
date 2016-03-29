#include <iostream>
#include <fstream>
#include <dirent.h>
#include "HouseManager.h"
using namespace std;

#define HOUSE_EXT ".house"
#define HOUSE_EXT_SIZE 6

HouseManager::HouseManager(const std::string& housesPath): numValidHouses(0){

	// fill the house with hard coded data - houses path not yet used
	houses.emplace_back(House());

	House& lastInsertedHouse = houses.back();

	bool isHouseReadSuccessful = lastInsertedHouse.readFromFile(housesPath + "/simple1.house");
	if (!isHouseReadSuccessful){
		// returning with numValidHouses = 0;
		return;
	}
	lastInsertedHouse.fixHouse();

	if (lastInsertedHouse.isHouseValid()){
		numValidHouses++;
	}
	if (numValidHouses == 0){
		cout << "Error: no valid house to run simulation. Quitting" << endl;
	}
}
