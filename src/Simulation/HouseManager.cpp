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
bool HouseManager::readHousesFiles(){
	// first, get a list of houses files, sorted alphabetically (full path)
	housesFileNames = FileUtils::getSortedFileNamesListBySuffix(housesPath, HOUSE_EXT);

	// if list is empty (error opening folder or no houses files in it) - print usage
	if (housesFileNames.empty()){
		ErrorPrinter::printUsage();
		cout << "cannot find house files in '" << FileUtils::getFullPath(housesPath) <<"'" << endl;
		return false;
	}
	return true;
}

void HouseManager::printHousesErrors(bool all){
	if (all){
		cout << "All house files in target folder '"
				<< FileUtils::getFullPath(housesPath)
				<< "' cannot be opened or are invalid:" << endl;
	}
	for(const pair<string, string>& houseErrPair : housesErrors) {
		cout << houseErrPair.first << ": " << houseErrPair.second << endl;
	}
}

void HouseManager::addHouseErr(const std::string& houseeFileName, const std::string& errorStr){
	housesErrors[houseeFileName] = errorStr;
}

void HouseManager::addValidHouse(const std::string& houseeFileName){
	validHousesFileNames.push_back(houseeFileName);
}



std::list<std::string>& HouseManager::getValidHousesFileNamesSorted(){
	validHousesFileNames.sort();
	return validHousesFileNames;

}


