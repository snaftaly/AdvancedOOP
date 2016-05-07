#ifndef SCOREMANAGER_H_
#define SCOREMANAGER_H_

#include <mutex>
#include <string>
#include <map>

typedef int (*func_ptr)(const std::map<std::string, int>&);

class ScoreManager {
	std::string scoreDirPath;
	void *dlib;
    func_ptr scorefunc_fptr;
    bool isScoreError;
	std::mutex scoreUpdateMutex;
	std::map<std::string, std::map<std::string, int>> algosScoresForHouses; // think of a better container

public:
	ScoreManager(const std::string& _scoreFilePath): scoreDirPath(_scoreFilePath), dlib(NULL), scorefunc_fptr(NULL), isScoreError(false){ }
	~ScoreManager();

	bool loadFormula();
	int calcScore(bool isMadeIllegalMove, const std::map<std::string, int>& scoreParams);
	void updateScore(const std::string& algoName, const std::string& houseFileName, int score);

	// getters
	const std::map<std::string, std::map<std::string, int> >& getAlgosScoresForHouses() const {
		return algosScoresForHouses;
	}
};

#endif /* SCOREMANAGER_H_ */
