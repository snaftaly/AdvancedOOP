#include <iostream>
#include <cstring>
#include "Simulator.h"
using namespace std;


int main(int argc, char* argv[]) {

	// default location of files
	string housesDir = ".";
	string configDir = ".";

	// check number of arguments
	if (argc != 1 && argc != 3 && argc != 5){
		cout << "Error: number of arguments is incorrect. Quitting." << endl;
		return 1;
	}
	// parse the arguments of the program and check parameter names
	for (int i = 1; i < argc; i=i+2){
		if (!strcmp("-config", argv[i])){
			configDir = argv[i+1];
		}
		else if (!strcmp("-house_path", argv[i])){
			housesDir = argv[i+1];
		}
		else {
			cout << "Error: wrong parameter name. Quitting." << endl;
			return 1;
		}
	}

	Simulator simulator(configDir, housesDir);

	if (!simulator.isInitSuccessfull()){
		// initialization of config file and house was not successful
		return 1;
	}
	simulator.runSimulation();
	simulator.printAlgosScores();

	return 0;
}
