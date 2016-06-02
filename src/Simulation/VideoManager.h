#ifndef VIDEOMANAGER_H_
#define VIDEOMANAGER_H_
#include <list>
#include <string>

class VideoManager {
	std::list<std::string> dirCreationErrors;
	std::list<std::string> imageCreationErrors;
	std::list<std::string> videoCreationErrors;

public:
	VideoManager();
	virtual ~VideoManager();

	void addDirCreationError(const std::string & algoName, const std::string & houseName, const std::string & path);
	void addImageCreationErrors(const std::string & algoName, const std::string & houseName, int numErrors);
	void addVideoCreationError(const std::string & algoName, const std::string & houseName);
	void printErrors();
};

#endif /* VIDEOMANAGER_H_ */
