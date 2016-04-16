#include "FileUtils.h"
#include <algorithm>
#include <dirent.h>
#include <iostream>
#include <stdlib.h>

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

string FileUtils::getFileNameFromPath(const std::string& filePath)
{
	int found = filePath.find_last_of("/");
	if (found < 0){
		return filePath;
	}
	return filePath.substr(found+1);
}

string FileUtils::getFileNameNoExt(const std::string& filePath)
{
	string fileName = getFileNameFromPath(filePath);
	int found = fileName.find_last_of(".");
	if  (found < 0){
		return fileName;
	}
	return fileName.substr(0, found);
}

string FileUtils::getFullPath(const std::string& path){
	char *fullPath = realpath(path.c_str(), NULL);
	if (fullPath == NULL){
		return path;
	}
	string fullPathStr(fullPath);
	free(fullPath);
	return fullPathStr;
}
