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

// our global factory for making shapes
map<string, maker_t *, less<string> > factory;

AlgorithmManager::~AlgorithmManager(){
	// delete from memory all the dynamically created algorithms
	for (AlgorithmRunner& algoRunner : algorithmRunnerList){
		delete algoRunner.getAlgorithm();
	}
	// now delete the so that was registered
	for(auto itr = dl_list.begin(); itr!=dl_list.end(); itr++){
		dlclose(*itr);

	}


}

bool AlgorithmManager::readAlgoFiles(){
	algorithmsFileNamesLst = FileUtils::getSortedFileNamesListBySuffix(algorithmsPath, ALGO_EXT);

	if (algorithmsFileNamesLst.empty()){
		ErrorPrinter::printUsage();
		return false;
	}

	void *dlib;
	int currFactoryMapSize, newFactoryMapSize;
	for (string& algoFileName : algorithmsFileNamesLst){
		currFactoryMapSize = factory.size();
		string algoPath = algorithmsPath + algoFileName;
		dlib = dlopen(algoPath.c_str(), RTLD_NOW);
		if(dlib == NULL){
			algorithmsLoadErrors[algoFileName] = "file cannot be loaded or is not a valid .so";
			continue;
		}
		dl_list.insert(dl_list.end(), dlib); // for later free of memory
		newFactoryMapSize = factory.size();
		if (currFactoryMapSize == newFactoryMapSize){
			algorithmsLoadErrors[algoFileName] = "valid .so but no algorithm was registered after loading it";
		}
	}

	if (factory.size() == 0){ // no algorithms registered
		printAlgorithmsErrors(true);
		return false;
	}
	return true;
}


void AlgorithmManager::createAlgorithmRunnerList(ConfigManager& confMgr){
	for (auto const & algorithmMakerPair : factory){
		AbstractAlgorithm * algo = algorithmMakerPair.second();
		algo->setConfiguration(confMgr.getConfs());
		algorithmRunnerList.emplace_back(algo, algorithmMakerPair.first);
	}
}


void AlgorithmManager::printAlgorithmsErrors(bool all){
	if (all){
		cout << "All algorithm files in target folder '"
				<< FileUtils::getFullPath(algorithmsPath)
				<< "' cannot be opened or are invalid:" << endl;
	}
	for(const pair<string, string>& algorithmErrPair : algorithmsLoadErrors) {
		cout << algorithmErrPair.first << ": " << algorithmErrPair.second << endl;
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
