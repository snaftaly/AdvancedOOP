#include <iostream>
#include <fstream>
#include <dirent.h>
#include "HouseManager.h"
#include "ErrorPrinter.h"
#include "FileUtils.h"
using namespace std;

#define HOUSE_EXT ".house"

/**
 * Read houses from path and return false on error
 */
// TODO: maybe split this function into two
bool HouseManager::readHousesFiles(){
	// first, get a list of houses files, sorted alphabetically (full path)
	housesFileNamesLst = FileUtils::getSortedFileNamesListBySuffix(housesPath, HOUSE_EXT);

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
		houses.push_back(std::move(tempHouse)); // push good tempHouse using move ctor

	}

	if (houses.empty()){
		printHousesErrors(true);
		return false;
	}
	return true;
}

void HouseManager::printHousesErrors(bool all){ // TODO: get full path
	if (all){
		cout << "All house files in target folder '"
				<< FileUtils::getFullPath(housesPath)
				<< "' cannot be opened or are invalid:" << endl;
	}
	for(const pair<string, string>& houseErrPair : housesErrors) {
		cout << houseErrPair.first << ": " << houseErrPair.second << endl;
	}
}
