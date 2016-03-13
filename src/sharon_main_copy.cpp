#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
#include "House.h"
#include "ConfigManager.h"
#include "HouseManager.h"

//
//int main(int argc, char* argv[]) {
//
//	// TODO: what should be done in case the arguments are not correct?
//	// default location of files
//	string housesDir = ".";
//	string configDir = ".";
//
//	// parse the arguments of the program
//	for (int i = 1; i < argc; i=i+2){
//		if (!strcmp("-config", argv[i])){
//			configDir = argv[i+1];
//		}
//		else if (!strcmp("-house_path", argv[i])){
//			housesDir = argv[i+1];
//		}
//	}
//
//	// get default confs in conMgr
//	ConfigManager confMgr;
//
//	// get all houses with HouseManager
//	HouseManager houseMgr(housesDir);
//
//	// print all houses
//	for (int i = 0; i< houseMgr.getHouseVector()->size(); i++){
//		houseMgr.getHouseVector()->at(i).print();
//	}
//	cout << "Bye" << endl;
//
//}
