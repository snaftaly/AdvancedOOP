#include "ErrorPrinter.h"
#include <iostream>
#include "FileUtils.h"


using namespace std;

void ErrorPrinter::printUsage(){
	cout << "Usage: simulator [-config <config path>] [-house_path <house path>]"
			" [-algorithm_path <algorithm path>] [-score_formula <score .so path>]"
			" [-threads <num threads>]" << endl;
}

void ErrorPrinter::cantFindFiles(const std::string& fileType, const std::string& fileDir){
	cout << "cannot find "<< fileType << " files in '" << FileUtils::getFullPath(fileDir) << "'"<< endl;
}

void ErrorPrinter::fileNotFound(const std::string& fileName, const std::string& fileDir){
	cout << "cannot find "<< fileName<< " file in '" << FileUtils::getFullPath(fileDir) << "'" << endl;

}

void ErrorPrinter::cantOpenFile(const std::string& fileName, const std::string& fileDir, bool isSO){
	cout << fileName << " exists in '" << FileUtils::getFullPath(fileDir) << "' but cannot be opened";
	if (isSO){
		cout << " or is not a valid .so";
	}
	cout << endl;
}

void ErrorPrinter::noValidFormula(){
	cout << "score_formula.so is a valid .so but it does not have a valid score formula" << endl;
}
