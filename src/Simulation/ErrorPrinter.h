#ifndef ERRORPRINTER_H_
#define ERRORPRINTER_H_

# include <string>

class ErrorPrinter {
public:
	static void printUsage();
	static void cantFindFiles(const std::string& fileType, const std::string& fileDir);
	static void fileNotFound(const std::string& fileName, const std::string& fileDir);
	static void cantOpenFile(const std::string& fileName, const std::string& fileDir, bool isSO);
	static void noValidFormula();
};

#endif /* ERRORPRINTER_H_ */
