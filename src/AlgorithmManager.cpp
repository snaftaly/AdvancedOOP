#include "AlgorithmManager.h"
#include "AlgosCommon.h"
#include "FileUtils.h"
#include "ErrorPrinter.h"

#include <dlfcn.h>
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


// TODO: remove d'tor
AlgorithmManager::~AlgorithmManager(){
	// delete from memory all the dynamically created algorithms
	// TODO remove all this
//	for (AlgorithmRunner& algoRunner : algorithmRunnerList){
//		delete algoRunner.getAlgorithm();
//	}
//	// now delete the so that was registered
//	for(auto itr = dl_list.begin(); itr!=dl_list.end(); itr++){
//		dlclose(*itr);
//
//	}
}

bool AlgorithmManager::readAlgoFiles(){
	// first get a list of file names
	algorithmsFileNamesLst = FileUtils::getSortedFileNamesListBySuffix(algorithmsPath, ALGO_EXT);

	if (algorithmsFileNamesLst.empty()){
		ErrorPrinter::printUsage();
		cout << "cannot find algorithm files in '" << FileUtils::getFullPath(algorithmsPath) << "'"<< endl;
		return false;
	}

	AlgorithmRegistrar& registrar = AlgorithmRegistrar::getInstance();

	for (const auto &algoFileName : algorithmsFileNamesLst){
		AlgoRegState result = registrar.loadAlgorithm(algorithmsPath, algoFileName);
        if(result != AlgoRegState::ALGORITHM_REGISTERED_SUCCESSFULY) {
            // handle errors in loading algorithm - add it to list of errors
        	algorithmsLoadErrors[algoFileName] = result;
        }
	}
//	for (string& algoFileName : algorithmsFileNamesLst){
//		currFactoryMapSize = factory.size();
//		string algoPath = algorithmsPath + algoFileName;
//		dlib = dlopen(algoPath.c_str(), RTLD_NOW);
//		if(dlib == NULL){
//			algorithmsLoadErrors[algoFileName] = "file cannot be loaded or is not a valid .so";
//			continue;
//		}
//		dl_list.insert(dl_list.end(), dlib); // for later free of memory
//		newFactoryMapSize = factory.size();
//		if (currFactoryMapSize == newFactoryMapSize){
//			algorithmsLoadErrors[algoFileName] = "valid .so but no algorithm was registered after loading it";
//		}
//	}

	if (registrar.size() == 0){ // no algorithms registered
		printAlgorithmsErrors(true);
		return false;
	}
	return true;
}


void AlgorithmManager::createAlgorithmRunnerList(ConfigManager& confMgr){
	// TODO: we will have to create such list for each thread
	AlgorithmRegistrar& registrar = AlgorithmRegistrar::getInstance();

    auto algorithms = registrar.getAlgorithms();
    auto& algorithmNames = registrar.getAlgorithmNames();
    auto pName = algorithmNames.begin();
    for(auto& algo: algorithms) {
		algo->setConfiguration(confMgr.getConfs());
		algorithmRunnerList.emplace_back(algo, *(pName++));
    }
//	for (auto const & algorithmMakerPair : factory){
//		AbstractAlgorithm * algo = algorithmMakerPair.second();
//		algo->setConfiguration(confMgr.getConfs());
//		algorithmRunnerList.emplace_back(algo, algorithmMakerPair.first);
//	}
}


void AlgorithmManager::printAlgorithmsErrors(bool all){
	if (all){
		cout << "All algorithm files in target folder '"
				<< FileUtils::getFullPath(algorithmsPath)
				<< "' cannot be opened or are invalid:" << endl;
	}
	string errStr;
	// TODO: maybe add a << operator to the enum class??
	for(const pair<string, string>& algorithmErrPair : algorithmsLoadErrors) {
		switch (algorithmErrPair.second){
			case(AlgoRegState::FILE_CANNOT_BE_LOADED) :
				errStr =  "file cannot be loaded or is not a valid .so";
				break;
			case (AlgoRegState::FILE_CANNOT_BE_LOADED) :
				errStr = "valid .so but no algorithm was registered after loading it";
				break;
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
