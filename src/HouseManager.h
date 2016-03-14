#ifndef HOUSEMANAGER_H_
#define HOUSEMANAGER_H_
#include <string>
#include <vector>
#include "House.h"

class HouseManager {

	//private data members
	std::vector<House> * houseVector;

	// private function
	static bool hasHouseExtension(char * filename);
	static void houseInput(House& house, std::string fileName);
	void getHouses(std::string housesDir);
public:
	// public data members
	int numHouses;

	// public function declerations
	// HouseManager c'tor signature:
	HouseManager(std::string &path);

	// HouseManager d'tor signature
	~HouseManager();

	// get the house vector
	std::vector<House> * getHouseVector(){ return houseVector; }
};

#endif /* HOUSEMANAGER_H_ */
