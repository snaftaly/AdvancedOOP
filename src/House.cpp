#include <iostream>
#include <fstream>
#include "House.h"
using namespace std;

// House c'tor - only initialized the matrix to NULL in the init list
House::House(): name(""), desc(""), rows(0), cols(0), houseMatrix(NULL) {

}

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

// House assignment operator implementation
House& House::operator=(const House& house)
{
    if(this != &house) {
    	if (houseMatrix != NULL){
    		delete [] houseMatrix;
    	}
		name = house.name;
		desc = house.desc;
		rows = house.rows;
		cols = house.cols;
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

std::ostream& operator<<(std::ostream& out, const House& house)
{
	return out << "Name: " << house.getName() << endl
			   << "Desc: " << house.getDesc() << endl
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
	return (numDocksInHouse == 1);
}

bool House::readFromFile(string fileName)
{
	ifstream fin(fileName.c_str());
	if (!fin.is_open()){
		cout << "Error opening house file: " <<  fileName << ". Quitting." << endl;
		return false;
	}
	getline(fin, name);
	getline(fin, desc);
	fin >> rows;
	fin >> cols;
	fin.ignore(); //skip newline and go to the beginning of matrix
	if (houseMatrix != NULL){
		delete [] houseMatrix;
	}
	houseMatrix = new string[rows];
	for (int i =0; i < rows; ++i)
	{
	    std::getline(fin, houseMatrix[i]);
	}
	if (fin.bad()){
		cout << "Error while reading house file " << fileName << ". Quitting." << endl;
		fin.close();
		return false;
	}
	fin.close();
	return true;
}
