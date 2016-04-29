#ifndef ALGORITHMREGISTRAR_H_
#define ALGORITHMREGISTRAR_H_

#include <iostream>
#include <functional>
#include <memory>
#include <list>
#include <cassert>
#include "AlgoRegState.h"

class AlgorithmRegistrar {
    list<void *> dl_list;
    std::list<std::string> algorithmNames;
    std::list<std::function<std::unique_ptr<AbstractAlgorithm>()>> algorithmFactories;
    void registerAlgorithm(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
        instance.algorithmFactories.push_back(algorithmFactory);
    }
    void setNameForLastAlgorithm(const std::string& algorithmName) {
//        assert(algorithmFactories.size()-1 == algorithmNames.size()); //TODO: what should be done here???
        algorithmNames.push_back(algorithmName);
    }
    ~AlgorithmRegistrar();
public:
    friend class AlgorithmRegistration;
    int loadAlgorithm(const std::string& path, const std::string& so_file_name_without_so_suffix);
    std::list<std::unique_ptr<AbstractAlgorithm>> getAlgorithms()const {
        std::list<std::unique_ptr<AbstractAlgorithm>> algorithms;
        for(auto algorithmFactoryFunc : algorithmFactories) {
            algorithms.push_back(algorithmFactoryFunc());
        }
        return algorithms;
    }
    const std::list<std::string>& getAlgorithmNames()const {
        return algorithmNames;
    }
    size_t size()const {
        return algorithmFactories.size();
    }
    static AlgorithmRegistrar& getInstance() {
        return instance;
    }

	list<void*>& getDlList() {
		return dl_list;
	}

private:
    static AlgorithmRegistrar instance;
};

#endif /* ALGORITHMREGISTRAR_H_ */
