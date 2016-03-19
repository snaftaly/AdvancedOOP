#include <iostream>
#include "House.h"
using namespace std;

// House c'tor - only initialized the matrix to NULL in the init list
House::House(): name(""), desc(""), rows(0), cols(0), houseMatrix(NULL) { }

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
		houseMatrix = new string[rows];
		for (int i = 0; i < rows; i++){
			houseMatrix[i] = house.houseMatrix[i];
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





