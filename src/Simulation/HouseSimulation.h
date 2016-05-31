#ifndef HOUSESIMULATION_H_
#define HOUSESIMULATION_H_

#include "House.h"
#include "AlgorithmRunner.h"
#include "AlgorithmManager.h"
#include "ScoreManager.h"
#include "VideoManager.h"

class HouseSimulation {

	int currSuccessfullAlgoPosition;
	int numSuccessfulAlgosInRound;
	int winnerNumSteps;
	int numStepsRemaining;
	int numAlogsRunning;
	int simulationSteps;
	int maxStepsAfterWinner;
	bool isThereAWinner;
	bool isUpdatedAboutToFinish;

public:
	HouseSimulation();
	~HouseSimulation();

	void runSimulationForHouse(AlgorithmManager& algoMgr, ScoreManager& scoreMgr, VideoManager& videoMgr, const House& house,
			std::list<AlgorithmRunner>& algoRunnerList, const int maxStepsAfterWinner, bool doVideo);
	void updateOnSuccessfulAlgo(AlgorithmRunner& successAlgorithmRunner, list<AlgorithmRunner>& algoRunnerList);
	void updateAboutToFinish(list<AlgorithmRunner>& algoRunnerList);

};

#endif /* HOUSESIMULATION_H_ */
