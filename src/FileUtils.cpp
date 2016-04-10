#include "FileUtils.h"
#include <algorithm>
#include <dirent.h>
using namespace std;

void FileUtils::addTrailingSlash(string& pathStr){
	if (pathStr.back() != '/'){
		pathStr += '/';
	}
}

bool FileUtils::endsWith(const string & str, const string & suffix){
	if (suffix.size() > str.size()) return false;
	return equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}


list<string> FileUtils::getSortedFileNamesListBySuffix(const string & path, const string & suffix){
	list<string> fileNamesList;

	DIR * dir = opendir(path.c_str());
	if (dir == NULL){
		return fileNamesList;
	}
	struct dirent *dp;
	while ((dp = readdir(dir)) != NULL){
		if (endsWith(string(dp->d_name), suffix)){
			fileNamesList.emplace_back(dp->d_name);
		}
	}
	closedir(dir);
	fileNamesList.sort();
	return fileNamesList;
}
