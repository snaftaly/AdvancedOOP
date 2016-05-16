#include <dlfcn.h>
#include <string>
#include <unistd.h>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "ScoreManager.h"
#include "ErrorPrinter.h"
#include "FileUtils.h"

#define HOUSE_NAME_MAX 9
#define ALGO_NAME_MAX 12
#define FIRST_COLUMN_WIDTH 13
#define OTHER_COLUMN_WIDTH 10

using namespace std;

int defaultFormula(const std::map<std::string, int>& resMap){

	int actual_position_in_competition;
	int winner_num_steps;
	int this_num_steps;
	int sum_dirt_in_house;
	int dirt_collected;
	int is_back_in_docking;

	int position_in_competition;
	int score;
	// check if one of the params don't exist
	if (resMap.find("actual_position_in_competition") == resMap.end() ||
		resMap.find("winner_num_steps") == resMap.end() ||
		resMap.find("this_num_steps") == resMap.end() ||
		resMap.find("sum_dirt_in_house") == resMap.end() ||
		resMap.find("dirt_collected") == resMap.end() ||
		resMap.find("is_back_in_docking") == resMap.end())
	{
		// one of the params is missing, return -1;
		return -1;
	}

	actual_position_in_competition = resMap.find("actual_position_in_competition")->second;
	winner_num_steps = resMap.find("winner_num_steps")->second;
	this_num_steps = resMap.find("this_num_steps")->second;
	sum_dirt_in_house = resMap.find("sum_dirt_in_house")->second;
	dirt_collected = resMap.find("dirt_collected")->second;
	is_back_in_docking = resMap.find("is_back_in_docking")->second;
	for (const auto& pair: resMap){
		cout << pair.first << ": " << pair.second << endl;
	}
	if (sum_dirt_in_house == dirt_collected && is_back_in_docking){
		position_in_competition = min(4, actual_position_in_competition);
		cout << "position_in_competition: " << position_in_competition << endl;
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
	cout << "pos:" << position_in_competition << endl;
	cout << "score: "<< score << endl;
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
		ErrorPrinter::printUsage();
		ErrorPrinter::fileNotFound("score_formula.so", scoreDirPath);
		return false;
	}

	dlib = dlopen(scorefuncFilePath.c_str(), RTLD_NOW);
	if(dlib == NULL){
		ErrorPrinter::printUsage();
		ErrorPrinter::cantOpenFile("score_formula.so", scoreDirPath, true);
		return false;
	}

    // reset errors
    dlerror();

    void *tempptr = dlsym(dlib, "calc_score");
    scorefunc_fptr = reinterpret_cast<func_ptr>(reinterpret_cast<long>(tempptr));
    const char *dlsym_error = dlerror();
    if (dlsym_error) {
		ErrorPrinter::printUsage();
		ErrorPrinter::noValidFormula();
		return false;
	}
	return true;
}

int ScoreManager::calcScore(bool isMadeIllegalMove, const std::map<std::string, int>& scoreParams){
	if (isMadeIllegalMove){
		return 0;
	}
	int score = scorefunc_fptr(scoreParams);
	if (score == -1){
		isScoreError = true;
	}
	return score;
}


void ScoreManager::updateScore(const std::string& algoName, const std::string& houseFileNameNoExt, int score){
	std::lock_guard<std::mutex> guard(scoreUpdateMutex); // use mutex for this part
	//search for algo in list

	auto result = std::find_if(algoScoresLst.begin(), algoScoresLst.end(), [algoName] (const algoHouseScores& s) { return s.getAlgoName() == algoName; });
	// if found - add to map
	if (result != algoScoresLst.end()){
		(*result).addHouseScore(houseFileNameNoExt, score);
	}
	// if not found create a new one
	else {
		map<string, int> tempMap;
		tempMap[houseFileNameNoExt] = score;
		algoScoresLst.push_back(algoHouseScores(algoName, tempMap));
	}

}

void ScoreManager::printError(){
	if (isScoreError){
		cout << "Score formula could not calculate some scores, see -1 in the results table" << endl;
	}
}


void ScoreManager::printRowSeparator(const int tableWidth){
	cout << setw(tableWidth) << setfill('-') << "" << endl << setfill(' ');;
}

void ScoreManager::printTableHeader(const list<string>& validHousesFileNamesSorted, const int tableWidth){
	//print row separator
	printRowSeparator(tableWidth);
	// print houses names
	cout << "|             |";
	for (const string& houseFileName : validHousesFileNamesSorted){
		string name =  FileUtils::getFileNameNoExt(houseFileName);
		name.resize(HOUSE_NAME_MAX, ' ');
		cout << name << " |";
	}
	cout << std::left << std::setw(10) << "AVG" << "|" << endl;
	//print row separator
	printRowSeparator(tableWidth);
}

void ScoreManager::printScoreTable(const list<string>& validHousesFileNamesSorted){
	int tableWidth = FIRST_COLUMN_WIDTH + 2 + (OTHER_COLUMN_WIDTH + 1)*(validHousesFileNamesSorted.size()+1);
	int scoreAlgoHouse;
	printTableHeader(validHousesFileNamesSorted, tableWidth);

	// sort the algo score (by avg and then by algo name
	algoScoresLst.sort();

	// print the results
	for (const auto& algoScoreObj : algoScoresLst){
		string algoNameTrimmed = algoScoreObj.getAlgoName();
		algoNameTrimmed.resize(ALGO_NAME_MAX, ' ');
		cout << "|" << algoNameTrimmed << " |";
		// print each house score
		for (const string& houseFileName : validHousesFileNamesSorted){
			scoreAlgoHouse = algoScoreObj.getHouseScores().find(FileUtils::getFileNameNoExt(houseFileName))->second;
			cout <<  right <<  setw(10) << scoreAlgoHouse << "|";
		}

		//print row separator
		cout << fixed << setprecision(2) << right<< setw(10) << algoScoreObj.getAvg()  << "|" << endl;
		printRowSeparator(tableWidth);
	}
}


// inner class related functions
bool operator==(const ScoreManager::algoHouseScores& lhs, const ScoreManager::algoHouseScores& rhs){
	return lhs.algoName == rhs.algoName;
}

const float ScoreManager::algoHouseScores::getAvg() const{
	int sum = 0;
	for (const auto& pair : houseScores){
		sum += pair.second;
	}
	return (float)sum/houseScores.size();
}

