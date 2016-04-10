#ifndef FILEUTILS_H_
#define FILEUTILS_H_

#include <list>
#include <string>

class FileUtils {
public:
	static bool endsWith(const std::string & str, const std::string & suffix);
	static std::list<std::string> getSortedFileNamesListBySuffix(const std::string & path, const std::string & suffix);
	static void addTrailingSlash(std::string& pathStr);
};

#endif /* FILEUTILS_H_ */
