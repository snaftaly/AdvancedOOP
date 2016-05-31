#include "VideoManager.h"

using namespace std;

VideoManager::VideoManager() {
	// TODO Auto-generated constructor stub

}

VideoManager::~VideoManager() {
	// TODO Auto-generated destructor stub
}

void VideoManager::addDirCreationError(const string & algoName, const string & houseName, const string & path){
	string errorStr =  "Error: In the simulation " + algoName + ", " + houseName + ": folder creation " + path + " failed";
	dirCreationErrors.push_back(errorStr);
}

void VideoManager::addImageCreationErrors(const string & algoName, const string & houseName, int numErrors){
	string errorStr = "Error: In the simulation " + algoName + ", " + houseName + ": the creation of " + to_string(numErrors) + " images was failed";
	imageCreationErrors.push_back(errorStr);
}

void VideoManager::addVideoCreationError(const string & algoName, const string & houseName){
	string errorStr = "Error: In the simulation " + algoName + ", " + houseName + ": video file creation failed";
	videoCreationErrors.push_back(errorStr);
}
