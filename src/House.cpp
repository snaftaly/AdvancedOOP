#include <iostream>
#include "House.h"


// House c'tor - only initialized the matrix to NULL in the init list
House::House(): name(""), desc(""), rows(0), cols(0), houseMatrix(NULL) { }

// House d'tor implementation
House::~House(){
	if (houseMatrix != NULL){
		for (int i = 0; i < rows; i++){
			delete [] houseMatrix[i];
		}
		delete [] houseMatrix;
	}
}

// House copy c'tor implementation
House::House(const House& house): rows(0), cols(0), houseMatrix(NULL){
	*this = house;
}

// House assignment operator implementation
House& House::operator=(const House& house){
    if(this != &house) {
    	int i, j;
    	for (i = 0; i < rows; i++){
			delete [] houseMatrix[i];
		}
    	if (houseMatrix != NULL){
    		delete [] houseMatrix;
    	}
		name = house.name;
		desc = house.desc;
		rows = house.rows;
		cols = house.cols;
		houseMatrix = new char*[rows];
		for (i = 0; i < rows; i++){
			houseMatrix[i] = new char[cols];
			for (j = 0; j < cols; j++){
				houseMatrix[i][j] = house.houseMatrix[i][j];
			}
		}
    }
    return *this;
}

// House print implementation
void House::print()
{
	std::cout << "House name: " << name << std::endl;
	std::cout << "House description: " << desc << std::endl;
	std::cout << "House matrix: " << std::endl;

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			std::cout << houseMatrix[i][j];
		}
		std::cout << std::endl;
	}
}




