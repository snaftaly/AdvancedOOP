#include "ScoreManager.h"

ScoreManager::~ScoreManager() {
	// TODO Auto-generated destructor stub
}

void ScoreManager::updateScore(const std::string& algoName, const std::string& houseFileNameNoExt, int score){
	std::lock_guard<std::mutex> guard(scoreUpdateMutex); // use mutex for this part
	algosScoresForHouses[algoName][houseFileNameNoExt] = score;
}
