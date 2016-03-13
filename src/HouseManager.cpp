#include <iostream>
#include <fstream>
#include <dirent.h>
#include "HouseManager.h"
#include "House.h"
using namespace std;

#define HOUSE_EXT ".house"
#define HOUSE_EXT_SIZE 6

HouseManager::HouseManager(std::string &path) {
	houseVector = new vector<House>;
	numHouses = 0;
	getHouses(path);
}

HouseManager::~HouseManager() {
	delete houseVector; // TODO: how to clear the house vector?
}

void HouseManager::getHouses(std::string housesDirPath){

	struct dirent *entry;
	DIR *dp;
    dp = opendir(housesDirPath.c_str());

    if (dp == NULL) {
        perror("opendir: Path does not exist or could not be read.");
        return;
    }

    while ((entry = readdir(dp))){
    	if (hasHouseExtension(entry->d_name)){
    		numHouses++;
    		houseVector->resize(numHouses);
    		houseInput(houseVector->at(numHouses -1), housesDirPath + "/" + entry->d_name);
    	}
    }
    closedir(dp);
}

void HouseManager::houseInput(House& house, string fileName)
{
	ifstream fin(fileName.c_str());
	std::getline(fin, house.name);
	std::getline(fin, house.desc);
	fin >> house.rows;
	fin >> house.cols;
	fin.ignore(); //skip newline and go to the beginning of matrix
	house.houseMatrix = new char*[house.rows];
	for (int i =0; i < house.rows; ++i)
	{
		house.houseMatrix[i] = new char[house.cols];
		for (int j = 0; j < house.cols; j++)
		{
			fin.get(house.houseMatrix[i][j]);
		}
		fin.ignore(); //skip newline and go to the next line
	}
}

bool HouseManager::hasHouseExtension(char * filename){ // TODO: do this function in a c++ way
	if (strlen(filename) >= HOUSE_EXT_SIZE
			&& strcmp(filename + strlen(filename) - HOUSE_EXT_SIZE, HOUSE_EXT) == 0) {
	  return true;
	}
	return false;
}

