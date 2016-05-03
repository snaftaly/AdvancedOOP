#include <dlfcn.h>

#include "AlgorithmRegistrar.h"
#include "FileUtils.h"

using namespace std;

// create the static field
AlgorithmRegistrar AlgorithmRegistrar::instance;

// AlgorithmRegistrar d'tor to close all DLs
AlgorithmRegistrar::~AlgorithmRegistrar(){
	// delete from memory all the dynamically created algorithms
	// TODO: how will they be removed? (unique ptr)
//	for (AlgorithmRunner& algoRunner : algorithmRunnerList){
//		delete algoRunner.getAlgorithm();
//	}
//	// now delete the so that was registered

	algorithmFactories.clear();
	for(auto itr = instance.dl_list.begin(); itr!= instance.dl_list.end(); itr++){
		dlclose(*itr); // TODO: understand why we get this error!!!
	}
}

AlgoRegState AlgorithmRegistrar::loadAlgorithm(const std::string& path, const std::string& so_file_name){

    size_t size = instance.size();
    void *dlib;
	string algoPath = path + so_file_name;
	dlib = dlopen(algoPath.c_str(), RTLD_NOW);
	if(dlib == NULL){
		return AlgoRegState::FILE_CANNOT_BE_LOADED;
	}

	instance.dl_list.insert(dl_list.end(), dlib);

    if(instance.size() == size) {
        return AlgoRegState::NO_ALGORITHM_REGISTERED; // no algorithm registered
    }

    instance.setNameForLastAlgorithm(FileUtils::getFileNameNoExt(so_file_name)); // TODO: remove .so extension now

    return AlgoRegState::ALGORITHM_REGISTERED_SUCCESSFULY;
}


list<unique_ptr<AbstractAlgorithm>> AlgorithmRegistrar::getAlgorithms()const {
    list<unique_ptr<AbstractAlgorithm>> algorithms;
    for(auto algorithmFactoryFunc : algorithmFactories) {
        algorithms.push_back(algorithmFactoryFunc());
    }
    return algorithms;
}
