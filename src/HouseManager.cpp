#include <iostream>
#include <fstream>
#include <dirent.h>
#include "HouseManager.h"
#include "ErrorPrinter.h"
#include "FileUtils.h"
using namespace std;

#define HOUSE_EXT ".house"
#define HOUSE_EXT_SIZE 6

/**
 * Read houses from path and return false on error
 */
bool HouseManager::readHousesFiles(){
	// first, get a list of houses files, sorted alphabetically (full path)
	housesFileNamesLst = FileUtils::getSortedFileNamesListBySuffix(housesPath, ".house");

	// if list is empty (error opening folder or no houses files in it) - print usage
	if (housesFileNamesLst.empty()){
		ErrorPrinter::printUsage();
		return false;
	}

	// for each house name in the list - create a house and if it's ok, put it in the list oh houses
	// if it's not ok - put the error in the error map
	for (string& houseFileName : housesFileNamesLst){
		House tempHouse;
		if (!(tempHouse.readFromFile(housesPath + houseFileName))){
			// there was an error reading from house from file, continue to next file
			housesErrors[houseFileName] = tempHouse.getErrStr();
			continue;
		}
		tempHouse.fixHouse(); // fix the number of columns+rows and walls
		if (!tempHouse.isHouseValid()){
			housesErrors[houseFileName] = tempHouse.getErrStr();
			continue;
		}
		// push the valid house to houses list using move ctor
		houses.push_back(std::move(tempHouse)); // TODO: check if move ctor/= operator works

	}

	if (houses.empty()){
		printHousesErrors(true);
		return false;
	}
	return true;
}

void HouseManager::printHousesErrors(bool all){
	if (all){
		cout << "All house files in target folder '"
				<< housesPath
				<< "' cannot be opened or are invalid:" << endl;
	}
	for(const pair<string, string>& houseErrPair : housesErrors) {
		cout << houseErrPair.first << ": " << houseErrPair.second << endl;
	}
}
