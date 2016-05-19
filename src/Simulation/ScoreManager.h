#ifndef SCOREMANAGER_H_
#define SCOREMANAGER_H_

#include <mutex>
#include <string>
#include <list>
#include <map>

typedef int (*func_ptr)(const std::map<std::string, int>&);

class ScoreManager {
    class algoHouseScores; // declaration of inner class to store each algo houses scores

	std::string scoreDirPath;
	void *dlib;
    func_ptr scorefunc_fptr;
    bool isScoreError;
	std::mutex scoreUpdateMutex;
    std::list<algoHouseScores> algoScoresLst;

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

	void printScoreTable(const std::list<std::string>& housesFileNamesSorted);
	void printRowSeparator(const int tableWidth);
	void printTableHeader(const std::list<std::string>& validHousesFileNamesSorted, const int tableWidth);
	void prepareScoresList(const std::list<std::string>& algorithmNames);


};

#endif /* SCOREMANAGER_H_ */
