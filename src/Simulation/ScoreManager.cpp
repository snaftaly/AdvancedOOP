#include <dlfcn.h>
#include <string>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include "ScoreManager.h"
#include "ErrorPrinter.h"
#include "FileUtils.h"

using namespace std;

int defaultFormula(const std::map<std::string, int>& resMap){
	cout << "inside ffault formula!!!!" << endl;

	for (const auto& pair : resMap){
		cout << pair.first << ": " << pair.second << endl;
	}
	int actual_position_in_competition;
//	int simulation_steps; // TODO: check default formula
	int winner_num_steps;
	int this_num_steps;
	int sum_dirt_in_house;
	int dirt_collected;
	int is_back_in_docking;

	int position_in_competition;
	int score;

	// check if one of the params don't exist
	if (resMap.find("actual_position_in_competition") == resMap.end() ||
//		resMap.find("simulation_steps") == resMap.end() ||
		resMap.find("winner_num_steps") == resMap.end() ||
		resMap.find("this_num_steps") == resMap.end() ||
		resMap.find("sum_dirt_in_house") == resMap.end() ||
		resMap.find("dirt_collected") == resMap.end() ||
		resMap.find("is_back_in_docking") == resMap.end())
	{
		cout << "return -1!!!!!!!!!!!!!!!!!!!" << endl;
		return -1;
	}

	actual_position_in_competition = resMap.find("actual_position_in_competition")->second;
//	simulation_steps = resMap.find("simulation_steps")->second;
	winner_num_steps = resMap.find("winner_num_steps")->second;
	this_num_steps = resMap.find("this_num_steps")->second;
	sum_dirt_in_house = resMap.find("sum_dirt_in_house")->second;
	dirt_collected = resMap.find("dirt_collected")->second;
	is_back_in_docking = resMap.find("is_back_in_docking")->second;

	if (sum_dirt_in_house == dirt_collected && is_back_in_docking){
		position_in_competition = min(4, actual_position_in_competition);
	}
	else{
		position_in_competition = 10;
	}

	score = max(0,
			2000
			- (position_in_competition - 1)*50
			+ (winner_num_steps - this_num_steps)*10
			- (sum_dirt_in_house - dirt_collected)*3
			+ (is_back_in_docking ? 50 : -200)
			);
	return score;
}

ScoreManager::~ScoreManager() {
	if (dlib != NULL){
		dlclose(dlib);
	}
}

bool ScoreManager::loadFormula(){
	if (scoreDirPath.empty()){
		scorefunc_fptr = &defaultFormula;
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

    // reset errors
    dlerror();

    void *tempptr = dlsym(dlib, "calc_score");
    scorefunc_fptr = reinterpret_cast<func_ptr>(reinterpret_cast<long>(tempptr));
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
		ErrorPrinter::printUsage();
		// TODO: put such errors in error printer
    	cout << "score_formula.so is a valid .so but it does not have a valid score formula" << endl;
		return false;
	}
	return true;
}

int ScoreManager::calcScore(bool isMadeIllegalMove, const std::map<std::string, int>& scoreParams){
	for (const auto& pair : scoreParams){
		cout << pair.first << ": " << pair.second << endl;
	}

	if (isMadeIllegalMove){
		return 0;
	}
	cout << "after isMadeIllegalMove !!!!!!!!!!!!!!" << endl;
	int score = scorefunc_fptr(scoreParams);
	if (score == -1){
		isScoreError = true;
	}
	return score;
}


void ScoreManager::updateScore(const std::string& algoName, const std::string& houseFileNameNoExt, int score){
	std::lock_guard<std::mutex> guard(scoreUpdateMutex); // use mutex for this part
	algosScoresForHouses[algoName][houseFileNameNoExt] = score;
}
