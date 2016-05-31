#ifndef FILEUTILS_H_
#define FILEUTILS_H_

#include <vector>
#include <string>

class FileUtils {
public:
	static bool endsWith(const std::string & str, const std::string & suffix);
	static std::vector<std::string> getSortedFileNamesListBySuffix(const std::string & path, const std::string & suffix);
	static void fixTrailingSlash(std::string& pathStr);
	static std::string getFileNameFromPath(const std::string& filePath);
	static const std::string getFileNameNoExt(const std::string& fileName);
	static std::string getFullPath(const std::string& fileName);
	static bool createDirectoryIfNotExists(const std::string& dirPath);
	static bool removeDir(const std::string& dirPath);

};

#endif /* FILEUTILS_H_ */
