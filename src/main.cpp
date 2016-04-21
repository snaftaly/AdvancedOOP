#include <iostream>
#include <cstring>
#include "Simulator.h"
#include "ErrorPrinter.h"
#include "FileUtils.h"
using namespace std;


int main(int argc, char* argv[]) {


	// default location of files
	string configDir = "./";
	string housesDir = "./";
	string algorithmsDir = "./";


	// parse the arguments of the program and check parameter names
	for (int i = 1; i < argc; ){
		if (!strcmp("-config", argv[i])){
			if (i < argc - 1){
				configDir = argv[i+1];
				i+=2;
			}
			else {
				ErrorPrinter::printUsage();
				return 1;
			}
		}
		else if (!strcmp("-house_path", argv[i])){
			if (i < argc - 1){
				housesDir = argv[i+1];
				i+=2;
			}
			else {
				ErrorPrinter::printUsage();
				return 1;
			}
		}
		else if (!strcmp("-algorithm_path", argv[i])){
			if (i < argc - 1){
				algorithmsDir = argv[i+1];
				i+=2;
			}
			else {
				ErrorPrinter::printUsage();
				return 1;
			}
		}
		else {
			i++;
		}
	}

	// add trailing '/' if it's missing
	FileUtils::fixTrailingSlash(configDir);
	FileUtils::fixTrailingSlash(housesDir);
	FileUtils::fixTrailingSlash(algorithmsDir);

	Simulator simulator(configDir, algorithmsDir, housesDir);

	if (!simulator.isInitSuccessfull()){
		// initialization of config file and house was not successful
		return 1;
	}

	simulator.runSimulation();
	simulator.printAlgosScores();
	simulator.printErrors();

	return 0;
}
