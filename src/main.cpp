#include <iostream>
#include <cstring>
#include "Simulator.h"
#include "ErrorPrinter.h"
#include "FileUtils.h"
using namespace std;


int main(int argc, char* argv[]) {

	// default location of files
	string configDir = ".";
	string housesDir = ".";
	string algorithmsDir = ".";

	// check number of arguments
	if (argc != 1 && argc != 3 && argc != 5 && argc != 7){
		ErrorPrinter::printUsage();
		return 1;
	}
	// parse the arguments of the program and check parameter names
	for (int i = 1; i < argc; i=i+2){
		if (!strcmp("-config", argv[i])){
			if (i < argc - 1){
				configDir = argv[i+1];
			}
			else {
				ErrorPrinter::printUsage();
				return 1;
			}
		}
		else if (!strcmp("-house_path", argv[i])){
			if (i < argc - 1){
				housesDir = argv[i+1];
			}
			else {
				ErrorPrinter::printUsage();
				return 1;
			}
		}
		else if (!strcmp("algorithm_path", argv[i])){
			if (i < argc - 1){
				algorithmsDir = argv[i+1];
			}
			else {
				ErrorPrinter::printUsage();
				return 1;
			}
		}
	}

	// add trailing '/' if it's missing // TODO: maybe move it somewhere else?
	FileUtils::addTrailingSlash(configDir);
	FileUtils::addTrailingSlash(housesDir);
	FileUtils::addTrailingSlash(algorithmsDir);

	Simulator simulator(configDir, housesDir, algorithmsDir);

	if (!simulator.isInitSuccessfull()){
		// initialization of config file and house was not successful
		cout << "init not successful. Bye" << endl;
		return 1;
	}
	simulator.runSimulation();
	simulator.printAlgosScores();

	cout << "Bye" << endl;
	return 0;
}
