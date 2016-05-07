#ifndef ALGORITHMMANAGER_H_
#define ALGORITHMMANAGER_H_
#include "../Common/Direction.h"
#include "../Common/AbstractAlgorithm.h"
#include "AlgorithmRunner.h"
#include "ConfigManager.h"
#include "AlgorithmRegistrar.h"



class AlgorithmManager {
	std::string algorithmsPath;
	std::vector<std::string> algorithmsFileNames;

	std::map<std::string, AlgoRegState> algorithmsLoadErrors;

	std::list<std::string> algorithmsRunErrors;

public:
	// AlgorithmManager ctor
	AlgorithmManager(const std::string& _algorithmsPath): algorithmsPath(_algorithmsPath){ }
	// AlgorithmManager dtor
	~AlgorithmManager(); // TODO: remove dtor
	bool readAlgoFiles();
	void printAlgorithmsErrors(bool all);
	void addAlgoRunError(const std::string & algoName,const std::string & houseName, int simulationStep);


	// getters

	const std::map<std::string, AlgoRegState>& getAlgorithmsLoadErrors() const {
		return algorithmsLoadErrors;
	}

	const std::list<std::string>& getAlgorithmsRunErrors() const {
		return algorithmsRunErrors;
	}
};

#endif /* ALGORITHMMANAGER_H_ */
