#ifndef ALGORITHMMANAGER_H_
#define ALGORITHMMANAGER_H_
#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "AlgorithmRunner.h"
#include "ConfigManager.h"


class AlgorithmManager {
	std::string algorithmsPath;
	std::list<std::string> algorithmsFileNamesLst;
	std::list<AbstractAlgorithm *> algorithmsList;
	std::map<std::string, std::string> algorithmsLoadErrors;
	std::list<std::string> algorithmsRunErrors;
	std::list<AlgorithmRunner> algorithmRunnerList;
	list<void *> dl_list; // list to hold handles for dynamic libs
public:
	// AlgorithmManager ctor
	AlgorithmManager(const std::string& _algorithmsPath): algorithmsPath(_algorithmsPath){ }
	// AlgorithmManager dtor
	~AlgorithmManager();
	bool readAlgoFiles();
	void printAlgorithmsErrors(bool all);
	void createAlgorithmRunnerList(ConfigManager& confMgr);


	// getters

	const std::map<std::string, std::string>& getAlgorithmsErrors() const {
		return algorithmsLoadErrors;
	}

	std::list<AlgorithmRunner>& getAlgorithmRunnerList() {
		return algorithmRunnerList;
	}
};

#endif /* ALGORITHMMANAGER_H_ */
