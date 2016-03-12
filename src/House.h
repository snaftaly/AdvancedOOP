#ifndef HOUSE_H
#define HOUSE_H

#include <cstring> // TODO: should we use string or cstring?

class House
{
public:
	std::string name;
	std::string desc;
	int rows;
	int cols;
	char ** houseMatrix;

	// House c'tor signature
	House();

	// House d'tor signature
	~House();

	// House copy c'tor
    House(const House& house);

	// House assignment operator
    House& operator=(const House& house);

    // House print signature
    void print();
};

#endif
