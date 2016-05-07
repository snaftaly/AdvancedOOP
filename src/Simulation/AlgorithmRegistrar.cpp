#include <dlfcn.h>

#include "AlgorithmRegistrar.h"
#include "FileUtils.h"

using namespace std;

// create the static field
AlgorithmRegistrar AlgorithmRegistrar::instance;

// AlgorithmRegistrar d'tor to close all DLs
AlgorithmRegistrar::~AlgorithmRegistrar(){
	// delete from memory all the dynamically created algorithm factories
	algorithmFactories.clear();
	// now delete the .so files that were registered

	for(auto itr = dl_list.begin(); itr!= dl_list.end(); itr++){
		dlclose(*itr);
	}
}

SORegState AlgorithmRegistrar::loadAlgorithm(const std::string& path, const std::string& so_file_name){

    size_t prev_size = size();
    void *dlib;
	string algoPath = path + so_file_name;
	dlib = dlopen(algoPath.c_str(), RTLD_NOW);
	if(dlib == NULL){
		return SORegState::FILE_CANNOT_BE_LOADED;
	}

	dl_list.insert(dl_list.end(), dlib);

    if(size() == prev_size) {
        return SORegState::NO_REGISTERED; // no algorithm registered
    }

    setNameForLastAlgorithm(FileUtils::getFileNameNoExt(so_file_name)); // remove .so extension now

    return SORegState::REGISTERED_SUCCESSFULY;
}


list<unique_ptr<AbstractAlgorithm>> AlgorithmRegistrar::getAlgorithms()const {
    list<unique_ptr<AbstractAlgorithm>> algorithms;
    for(auto algorithmFactoryFunc : algorithmFactories) {
        algorithms.push_back(algorithmFactoryFunc());
    }
    return algorithms;
}
