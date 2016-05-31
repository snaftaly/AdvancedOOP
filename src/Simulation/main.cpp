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
	string scoreFormulaDir = "";
	int numThreads = 1;
	bool doVideo = false;


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
		else if (!strcmp("-score_formula", argv[i])){
			if (i < argc - 1){
				scoreFormulaDir = argv[i+1];
				i+=2;
			}
			else {
				ErrorPrinter::printUsage();
				return 1;
			}
		}
		else if (!strcmp("-video", argv[i])){
			doVideo = true;
			i++;
		}
		else if (!strcmp("-threads", argv[i])){
			if (i < argc - 1){
				// try to get num threads, if it's NaN or <=0 then num Threads = 1
				try{
					numThreads = stoi(argv[i+1]);
				}
				catch (...){
					numThreads = 1;
				}
				if (numThreads <= 0){
					numThreads = 1;
				}
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

	if (doVideo && numThreads > 1){
		cout << "Error: Cannot create video when running multiple threads" << endl;
		return 1;
	}

	// add trailing '/' if it's missing
	FileUtils::fixTrailingSlash(configDir);
	if (!scoreFormulaDir.empty()){
		// score formula path was given
		FileUtils::fixTrailingSlash(scoreFormulaDir);
	}
	FileUtils::fixTrailingSlash(algorithmsDir);
	FileUtils::fixTrailingSlash(housesDir);


	Simulator simulator(configDir, scoreFormulaDir, algorithmsDir, housesDir,  numThreads);

	if (!simulator.isInitSuccessfull()){
		// initialization of config file and house was not successful
		return 1;
	}

	simulator.runSimulation(doVideo);
	simulator.printResults();

	return 0;
}
