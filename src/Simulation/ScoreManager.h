#ifndef SCOREMANAGER_H_
#define SCOREMANAGER_H_

#include <mutex>
#include <string>
#include <list>

typedef int (*func_ptr)(const std::map<std::string, int>&);

class ScoreManager {
    class algoHouseScores; // declaration of inner class

	std::string scoreDirPath;
	void *dlib;
    func_ptr scorefunc_fptr;
    bool isScoreError;
	std::mutex scoreUpdateMutex; // TODO: make sure we use mutex when updating the list
    std::list<algoHouseScores> lst;

    void addScoreToList(std::string algoName, std::string house, int score);
    void printListSorted(); // TODO: remove this

    // *** inner class definition ***
	class algoHouseScores{ // nested class
		std::string algoName;
		std::map<std::string, int> houseScores;
		friend bool operator==(const algoHouseScores& lhs, const algoHouseScores& rhs);
	public:
		// inner class c'tor
		algoHouseScores(std::string algo, std::map<std::string, int> m): algoName(algo), houseScores(m){}
		const float getAvg() const;
		void addHouseScore(const std::string& houseName, const int score){
			houseScores[houseName] = score;
		}
		bool operator < (const algoHouseScores& other) const { // sort by avg, and then by algorithm name
			return (this->getAvg() > other.getAvg() || this->algoName < other.algoName);
		}

		const std::string& getAlgoName() const {
			return algoName;
		}

		const std::map<std::string, int>& getHouseScores() const {
			return houseScores;
		}

	};
	friend bool operator==(const algoHouseScores& lhs, const algoHouseScores& rhs);

public:
	ScoreManager(const std::string& _scoreFilePath): scoreDirPath(_scoreFilePath), dlib(NULL), scorefunc_fptr(NULL), isScoreError(false){ }
	~ScoreManager();

	bool loadFormula();
	int calcScore(bool isMadeIllegalMove, const std::map<std::string, int>& scoreParams);
	void updateScore(const std::string& algoName, const std::string& houseFileName, int score);
	void printError();

};

#endif /* SCOREMANAGER_H_ */
