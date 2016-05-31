#include <iostream>
#include <fstream>
#include "House.h"
#include "FileUtils.h"
using namespace std;

// House c'tor - only initialized the matrix to NULL in the init list
House::House(): name(""), maxSteps(0), rows(0), cols(0), houseMatrix(nullptr), filePath(""), errStr("") { }

// House d'tor implementation
House::~House(){}

// House copy c'tor implementation
House::House(const House& house): rows(0), cols(0), houseMatrix(nullptr){
	*this = house;
}

// house Move c'tor implementation
House::House(House&& house): name(house.name), maxSteps(house.maxSteps),
		rows(house.rows), cols(house.cols), houseMatrix(nullptr),
		filePath(house.filePath), errStr(house.errStr){
	std::swap(houseMatrix, house.houseMatrix); // the swap trick
}

// House copy assignment operator implementation
House& House::operator=(const House& house)
{
    if(this != &house) {

		name = house.name;
		maxSteps = house.maxSteps;
		rows = house.rows;
		cols = house.cols;
		filePath = house.filePath;
		errStr = house.errStr;
		if (!house.houseMatrix){ // house's houseMatrix is nullPtr
			houseMatrix.reset(nullptr);
		}
		else {
			houseMatrix = make_unique<std::string[]>(rows);
			for (size_t i = 0; i < rows; i++){
				houseMatrix[i] = house.houseMatrix[i];
			}
		}
    }
    return *this;
}

// House move assignment operator implementation
House& House::operator=(House&& house){

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
	if (houseMatrix){
		for (size_t i = 0; i< rows; i++){
			retStr += houseMatrix[i];
			retStr += '\n';
		}
	}
	return retStr;
}

int House::calcDirtLevel () const{
	int dirtLevel = 0;
	char currPlace;
	if (!houseMatrix){
		return -1;
	}
	for (size_t i=0; i<rows; i++){
		for (size_t j=0; j<cols; j++){
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
	for (size_t i=0; i<rows; i++){
		for (size_t j=0; j<cols; j++){
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
	fixUnknownLetters();
	fixWalls();
}

void House::fixMissingRowsAndCols(){
	// This functions assumes that the matrix itself is in the correct size
	// fix rows that are too short
	for (size_t i = 0; i < rows; i++){
		size_t rowSize = houseMatrix[i].size();
		if (rowSize < cols){
			houseMatrix[i].insert(rowSize, cols - rowSize, ' ');
		}
	}

}

void House::fixUnknownLetters(){
	char currPlace;
	for (size_t i=0; i < rows; i++){
		for (size_t j=0; j < cols; j++){
			currPlace = houseMatrix[i][j];
			if ((currPlace >= '0' && currPlace <='9') || currPlace == ' ' || currPlace == 'D' || currPlace == 'W'){
				continue;
			}
			else {
				houseMatrix[i][j] = ' ';
			}

		}
	}

}

void House::fixWalls(){
	// fix walls in first row and last row
	for (size_t j=0; j < cols; j++){
		houseMatrix[0][j] = 'W';
		houseMatrix[rows-1][j] = 'W';
	}
	// fix walls in first column and last column
	for (size_t i=0; i<rows; i++){
		houseMatrix[i][0] = 'W';
		houseMatrix[i][cols-1] = 'W';
	}

}

bool House::isHouseValid(){
	int numDocksInHouse = 0;
	char currPlace;
	for (size_t i=0; i<rows; i++){
		for (size_t j=0; j<cols; j++){
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

	houseMatrix = make_unique<std::string[]>(rows);
	for (size_t i =0; i < rows; ++i)
	{
	    std::getline(fin, houseMatrix[i]);
	    houseMatrix[i].resize(cols, ' ');
	}
	fin.close();
	return true;
}

bool House::checkNumberInLine(int lineNumber, size_t * paramName, const string & lineStr){
	long value;
	try{
		value = stol(lineStr.c_str());
	}
	catch (...){
		value = -1; // -1 indicates that there was an error
	}
	if (value <= 0){
		errStr = "line number "	+
				to_string(lineNumber) +
				" in house file shall be a positive number, found: " +
				lineStr;
		return false;
	}
	else {
		*paramName = value;
		return true;
	}
}
