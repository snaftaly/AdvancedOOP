#include <iostream>
#include <fstream>
#include <dirent.h>
#include "HouseManager.h"
#include "ErrorPrinter.h"
using namespace std;

#define HOUSE_EXT ".house"
#define HOUSE_EXT_SIZE 6

/**
 * Read houses from path and return false on error
 */
bool HouseManager::readHousesFiles(){
	// first, get a list of houses files, sorted alphabetically (full path)
	housesPathsLst.emplace_back(housesPath + "simple1.house"); // TODO: change this to reading paths from dir

	// if list is empty (error opening folder or no houses files in it) - print usage
	if (housesPathsLst.empty()){
		ErrorPrinter::printUsage();
		return false;
	}

	// for each path in the list - create a house and if it's ok, put it in the list oh houses
	// if it's not ok - put the error in the error map TODO: do it


	// fill the house with hard coded data - houses path not yet used
	houses.emplace_back();

	House& lastInsertedHouse = houses.back();

	bool isHouseReadSuccessful = lastInsertedHouse.readFromFile(housesPath + "simple1.house");
	if (!isHouseReadSuccessful){
		// returning with numValidHouses = 0;
		return false;
	}
	lastInsertedHouse.fixHouse();

	if (lastInsertedHouse.isHouseValid()){
		numValidHouses++;
	}
	if (numValidHouses == 0){
		cout << "Error: no valid house to run simulation. Quitting" << endl;
	}
	return true;
}
