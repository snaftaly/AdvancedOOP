#ifndef FILEUTILS_H_
#define FILEUTILS_H_

#include <list>
#include <string>

class FileUtils {
public:
	static bool endsWith(const std::string & str, const std::string & suffix);
	static std::list<std::string> getSortedFileNamesListBySuffix(const std::string & path, const std::string & suffix);
	static void fixTrailingSlash(std::string& pathStr);
	static std::string getFileNameFromPath(const std::string& filePath);
	static const std::string getFileNameNoExt(const std::string& fileName);
	static std::string getFullPath(const std::string& fileName);
};

#endif /* FILEUTILS_H_ */
