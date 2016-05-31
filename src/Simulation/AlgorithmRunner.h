#ifndef ALGORITHMRUNNER_H_
#define ALGORITHMRUNNER_H_
#include <list>
#include <tuple>
#include <map>
#include <string>
#include <functional>
#include <memory>
#include "../Common/Direction.h"
#include "../Common/AbstractAlgorithm.h"
#include "../Common/AbstractSensor.h"
#include "VideoManager.h"
#include "Sensor.h"
#include "SimulationState.h"

class AlgorithmRunner {
	unique_ptr<AbstractAlgorithm> algorithm;
	Sensor sensor;
	std::string algoName;

	// current house info:
	House currHouse;
	int roboti, robotj;

	int batteryLevel;
	int numSteps;
	int dirtCollected;
	Direction prevStep = Direction::Stay;
	int currHouseTotDirt;

	int algoPositionInCompetition;
	SimulationState simulationState;


	bool doVideo;
	std::string imagesDir;
	int imagesCounter;
	int failedImagesCounter;

	static map<string, int> config;


	bool isLegalStep(int stepi, int stepj);

	void printSimulation(int stepi, int stepj);

public:

	AlgorithmRunner(unique_ptr<AbstractAlgorithm>& algo, string algoName);

	void setCurrHouse(const House& currHouse) {
		this->currHouse = currHouse;
	}

	Sensor& getSensor(){
		return sensor;
	}

	void resetRunnerForNewHouse(const House& house, int currHouseDocki, int currHouseDockj, int houseTotDirt);
	void setSensorForAlgorithm();

	bool isHouseCleanAndRobotInDock();
	bool isBatteryConsumedAndRobotNotInDock();
	bool getStepAndUpdateIfLegal(VideoManager& videoMgr);
	bool isRobotInDock();
	int getActualPositionInCompetition();


	void updateStepsRemainingOnWinner(int numStepsRemaining);

	const std::map<std::string, int> getScoreParams(const int winnerNumSteps, const int simulationSteps,
			const int unsuccessfullAlgosPosition);
	bool isMadeIllegalMove();
	void addStepImage(VideoManager& videoMgr);

	// setters


	void setAlgoRankInCompetition(int algoRankInCompetition) {
		this->algoPositionInCompetition = algoRankInCompetition;
	}

	void setSimulationState(SimulationState SimState) {
		this->simulationState = SimState;
	}

	// getters

	bool getIsFinished() const {
		return (this->simulationState != SimulationState::Running);
	}

	int getNumSteps() const {
		return numSteps;
	}

	SimulationState getFinishState() const {
		return simulationState;
	}

	const std::string& getAlgoName() const {
		return algoName;
	}

	int getFailedImagesCounter() const {
		return failedImagesCounter;
	}

	const std::string& getImagesDir() const {
		return imagesDir;
	}

	static void setConfig(const map<string, int>& conf) {
		config = conf;
	}

	void setDoVideo(bool doVideo) {
		this->doVideo = doVideo;
	}

	void setImageDir(const std::string& imageDir) {
		this->imagesDir = imageDir;
	}


};

#endif /* ALGORITHMRUNNER_H_ */
