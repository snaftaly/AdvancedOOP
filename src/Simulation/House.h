#ifndef HOUSE_H
#define HOUSE_H

#include <string>
#include <iostream>
#include <tuple>
#include "../Common/MakeUniqueAdder.h"

class House
{
	std::string name;
	size_t maxSteps;
	size_t rows;
	size_t cols;
	std::unique_ptr<std::string[]> houseMatrix;
	std::string filePath;
	std::string errStr;
public:

	// House c'tor signature
	House();

	// House d'tor signature
	~House();

	// House copy c'tor signature
    House(const House& house);

    // House move c'tor signature
    House(House&& house);

	// House copy assignment operator
    House& operator=(const House& house);

	// House move assignment operator
    House& operator=(House&& house);

    // House << operator
    friend std::ostream& operator<<(std::ostream& out, const House& p);

    int calcDirtLevel() const;

    std::tuple<int, int> getHouseDockPlace() const;

    bool readFromFile(std::string fileName);

    bool checkNumberInLine(int lineNumber, size_t * paramName, const std::string & lineStr);

	void fixHouse();

	void fixUnknownLetters();

	void fixMissingRowsAndCols();

	void fixWalls();

	bool isHouseValid();

    // getters

	const std::string getName() const {
		return name;
	}

	int getMaxSteps() const {
		return maxSteps;
	}

	int getRows() const {
		return rows;
	}

	int getCols() const {
		return cols;
	}

	const std::string& getFileName() const {
		return filePath;
	}

	const std::string getErrStr() const {
		return errStr;
	}

	std::unique_ptr<std::string[]>& getHouseMatrix() {
		return houseMatrix;
	}

	std::string getHouseMatrixStr() const;


	// setters

	void setName(const std::string& name) {
		this->name = name;
	}

	void setMaxSteps(int maxSteps) {
		this->maxSteps = maxSteps;
	}

	void setRows(int rows) {
		this->rows = rows;
	}

	void setCols(int cols) {
		this->cols = cols;
	}
};

#endif
