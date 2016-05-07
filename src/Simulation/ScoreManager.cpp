#include <dlfcn.h>
#include <string>
#include <unistd.h>
#include "ScoreManager.h"
#include "ErrorPrinter.h"
#include "FileUtils.h"

using namespace std;

ScoreManager::~ScoreManager() {
	if (dlib != NULL){
		dlclose(dlib);
	}
}

bool ScoreManager::loadFormula(){
	if (scoreDirPath.empty()){
		// not using the score from so file
		return true;
	}
	string scorefuncFilePath = scoreDirPath + "score_formula.so";

	if (( access( scorefuncFilePath.c_str(), F_OK ) == -1 )){
		// TODO: should it be the path of the folder or path to the file iteslf?
		ErrorPrinter::printUsage();
		// TODO: put such errors in error printer
		cout << "cannot find score_formula.so file in '" << FileUtils::getFullPath(scoreDirPath) << "'" << endl;
		return false;
	}

	dlib = dlopen(scorefuncFilePath.c_str(), RTLD_NOW);
	if(dlib == NULL){
		ErrorPrinter::printUsage();
		// TODO: put such errors in error printer
		cout << "score_formula.so exists in '" << FileUtils::getFullPath(scoreDirPath) << "' but cannot be opened or is not a valid .so" << endl;
		return false;
	}

	void *fptr =

	return true;
}


void ScoreManager::updateScore(const std::string& algoName, const std::string& houseFileNameNoExt, int score){
	std::lock_guard<std::mutex> guard(scoreUpdateMutex); // use mutex for this part
	algosScoresForHouses[algoName][houseFileNameNoExt] = score;
}
