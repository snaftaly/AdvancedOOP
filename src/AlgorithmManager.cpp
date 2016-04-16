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
	for(auto itr = dl_list.begin(); itr!=dl_list.end(); itr++){
		dlclose(*itr);
	}

	// delete from memory all the dynamically created algorithms
	for (AlgorithmRunner& algoRunner : algorithmRunnerList){
		delete algoRunner.getAlgorithm();
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
		string algoFullPath = algorithmsPath + algoFileName;
		dlib = dlopen(algoFullPath.c_str(), RTLD_NOW);
		if(dlib == NULL){
			algorithmsErrors[algoFileName] = "file cannot be loaded or is not a valid .so";
		}
		dl_list.insert(dl_list.end(), dlib); // for later free of memory // TODO: maybe we can free it here?
		newFactoryMapSize = factory.size();
		if (currFactoryMapSize == newFactoryMapSize){
			algorithmsErrors[algoFileName] = "valid .so but no algorithm was registered after loading it";
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
//	for (AbstractAlgorithm* algo : algorithms){
//		// set the configurations for the algorithm
//		algo->setConfiguration(confMgr.getConfs());
//
//		// put the algorithm in the algoRunner list
//		algorithmRunnerList.emplace_back(algo);
//	}
//	for (AlgorithmRunner& algorithmRunner : algorithmRunnerList){
//		algorithmRunner.setSensorForAlgorithm(); // TODO: move this to algorunner init (line above)
//	}
}


void AlgorithmManager::printAlgorithmsErrors(bool all){ // TODO: get full path
	if (all){
		cout << "All algorithm files in target folder '"
				<< FileUtils::getFullPath(algorithmsPath)
				<< "' cannot be opened or are invalid:" << endl;
	}
	for(const pair<string, string>& algorithmErrPair : algorithmsErrors) {
		cout << algorithmErrPair.first << ": " << algorithmErrPair.second << endl;
	}
}
