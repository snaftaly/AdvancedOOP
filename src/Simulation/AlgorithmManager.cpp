#include "AlgorithmManager.h"
#include "FileUtils.h"
#include "ErrorPrinter.h"

#include <iostream>
#include <map>
#include <list>
#include <vector>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define ALGO_EXT "_.so"

using namespace std;

AlgorithmManager::~AlgorithmManager(){

}

bool AlgorithmManager::readAlgoFiles(){
	// first get a list of file names
	algorithmsFileNames = FileUtils::getSortedFileNamesListBySuffix(algorithmsPath, ALGO_EXT);

	if (algorithmsFileNames.empty()){
		ErrorPrinter::printUsage();
		cout << "cannot find algorithm files in '" << FileUtils::getFullPath(algorithmsPath) << "'"<< endl;
		return false;
	}

	AlgorithmRegistrar& registrar = AlgorithmRegistrar::getInstance();

	for (const auto &algoFileName : algorithmsFileNames){
		SORegState result = registrar.loadAlgorithm(algorithmsPath, algoFileName);
        if(result != SORegState::REGISTERED_SUCCESSFULY) {
            // handle errors in loading algorithm - add it to list of errors
        	algorithmsLoadErrors[algoFileName] = result;
        }
	}

	if (registrar.size() == 0){ // no algorithms registered
		printAlgorithmsErrors(true);
		return false;
	}
	return true;
}


void AlgorithmManager::printAlgorithmsErrors(bool all){
	if (all){
		cout << "All algorithm files in target folder '"
				<< FileUtils::getFullPath(algorithmsPath)
				<< "' cannot be opened or are invalid:" << endl;
	}
	string errStr;
	for(const pair<string, SORegState>& algorithmErrPair : algorithmsLoadErrors) {
		if (algorithmErrPair.second == SORegState::FILE_CANNOT_BE_LOADED) {
				errStr =  "file cannot be loaded or is not a valid .so";
		}
		else if (algorithmErrPair.second == SORegState::NO_REGISTERED){
				errStr = "valid .so but no algorithm was registered after loading it";
		}
		cout << algorithmErrPair.first << ": " << errStr << endl;
	}

	for (const string& runErrStr: algorithmsRunErrors){
		cout << runErrStr << endl;
	}
}


void AlgorithmManager::addAlgoRunError(const string & algoName, const string & houseName, int simulationStep){
	string errorStr = "Algorithm "
			+ algoName
			+ " when running on House "
			+ houseName
			+ " went on a wall in step "
			+ to_string(simulationStep);
	algorithmsRunErrors.push_back(errorStr);
}
