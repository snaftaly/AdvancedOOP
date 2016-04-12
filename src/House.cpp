#include <iostream>
#include <fstream>
#include "House.h"
#include "FileUtils.h"
using namespace std;

// House c'tor - only initialized the matrix to NULL in the init list
House::House(): name(""), maxSteps(0), rows(0), cols(0), houseMatrix(NULL), filePath(""), errStr("") { }

// House d'tor implementation
House::~House()
{
	if (houseMatrix != NULL)
	{
		delete[] houseMatrix;
	}
}

// House copy c'tor implementation
House::House(const House& house): rows(0), cols(0), houseMatrix(NULL){
	*this = house;
}

// house Move c'tor implementation
House::House(House&& house): name(house.name), maxSteps(house.maxSteps),
		rows(house.rows), cols(house.cols), houseMatrix(NULL),
		filePath(house.filePath), errStr(house.errStr){
	std::swap(houseMatrix, house.houseMatrix); // the swap trick
}

// House copy assignment operator implementation
House& House::operator=(const House& house)
{
    if(this != &house) {
    	if (houseMatrix != NULL){
    		delete [] houseMatrix;
    	}
		name = house.name;
		maxSteps = house.maxSteps;
		rows = house.rows;
		cols = house.cols;
		filePath = house.filePath;
		errStr = house.errStr;
		if (house.houseMatrix == NULL){
			houseMatrix = NULL;
		}
		else {
			houseMatrix = new string[rows];
			for (int i = 0; i < rows; i++){
				houseMatrix[i] = house.houseMatrix[i];
			}
		}
    }
    return *this;
}

// House move assignment operator implementation
House& House::operator=(House&& house){

	if (houseMatrix != NULL){
		delete [] houseMatrix;
	}
	name = house.name;
	maxSteps = house.maxSteps;
	rows = house.rows;
	cols = house.cols;
	filePath = house.filePath;
	errStr = house.errStr;
	std::swap(houseMatrix, house.houseMatrix); // the swap trick
	return *this;
}

std::ostream& operator<<(std::ostream& out, const House& house)
{
	return out << "Name: " << house.getName() << endl
			<< "maxSteps: " << house.getName() << endl
			<< "Matrix: " << endl << house.getHouseMatrixStr();
}

std::string House::getHouseMatrixStr() const
{
	std::string retStr;
	if (houseMatrix != NULL){
		for (int i = 0; i< rows; i++){
			retStr += houseMatrix[i];
			retStr += '\n';
		}
	}
	return retStr;
}

int House::calcDirtLevel () const{
	int dirtLevel = 0;
	char currPlace;
	if (houseMatrix == NULL){
		return -1;
	}
	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			currPlace = houseMatrix[i][j] ;
			if (currPlace > '0' && currPlace <= '9'){
				dirtLevel += currPlace - '0';
			}
		}
	}
	return dirtLevel;
}

tuple<int, int> House::getHouseDockPlace() const {
	tuple<int, int> dockPlace{-1, -1};
	if (houseMatrix == NULL){
		return dockPlace;
	}
	char currPlace;
	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			currPlace = houseMatrix[i][j];
			if (currPlace == 'D'){
				get<0>(dockPlace) = i;
				get<1>(dockPlace) = j;
				return dockPlace;
			}
		}
	}
	return dockPlace;
}

void House::fixHouse(){
	fixMissingRowsAndCols();
	fixWalls();
}

void House::fixMissingRowsAndCols(){
	// This functions assumes that the matrix itself is in the correct size
	// fix rows that are too short
	for (int i = 0; i < rows; i++){
		int rowSize = houseMatrix[i].size();
		if (rowSize < cols){
			houseMatrix[i].insert(rowSize, cols - rowSize, ' ');
		}
	}

}

void House::fixWalls(){
	// fix walls in first row and last row
	for (int j=0; j < cols; j++){
		houseMatrix[0][j] = 'W';
		houseMatrix[rows-1][j] = 'W';
	}
	// fix walls in first column and last column
	for (int i=0; i<rows; i++){
		houseMatrix[i][0] = 'W';
		houseMatrix[i][cols-1] = 'W';
	}

}

bool House::isHouseValid(){
	int numDocksInHouse = 0;
	char currPlace;
	for (int i=0; i<rows; i++){
		for (int j=0; j<cols; j++){
			currPlace = houseMatrix[i][j];
			if (currPlace == 'D'){
				numDocksInHouse++;
			}
		}
	}
	if (numDocksInHouse < 1){
		errStr = "missing docking station (no D in house)";
	}
	else if(numDocksInHouse > 1){
		errStr = "too many docking stations (more than one D in house)";
	}
	return (numDocksInHouse == 1);
}

bool House::readFromFile(string filePath)
{
	this->filePath = filePath;

	// attemp to open file
	ifstream fin(filePath.c_str());
	if (!fin.is_open()){
		errStr = "cannot open file";
		return false;
	}
	getline(fin, name);
	string tempLine;

	// get the maxSteps Line and attempt to turn it into an integer
	getline(fin, tempLine); // TODO: should max steps be a positive number or >=0?
	if (!checkNumberInLine(2, &maxSteps, tempLine)) return false;

	getline(fin, tempLine);
	if (!checkNumberInLine(3, &rows, tempLine)) return false;

	getline(fin, tempLine);
	if (!checkNumberInLine(4, &cols, tempLine)) return false;

	if (houseMatrix != NULL){
		delete [] houseMatrix;
	}
	houseMatrix = new string[rows];
	for (int i =0; i < rows; ++i)
	{
	    std::getline(fin, houseMatrix[i]);
	}
	fin.close();
	return true;
}

bool House::checkNumberInLine(int lineNumber, int * paramName, const string & lineStr){
	bool isLineOK = true;
	try{
		*paramName = stoi(lineStr.c_str());
	}
	catch (...){
		isLineOK = false;
	}
	if (*paramName <= 0){
		isLineOK = false;
	}
	if (!isLineOK){
		errStr = "line number "	+
				to_string(lineNumber) +
				" in house file shall be a positive number, found: " +
				lineStr;
	}
	return isLineOK;
}
