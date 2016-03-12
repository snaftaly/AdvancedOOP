#ifndef HOUSE_H
#define HOUSE_H

#include <cstring> // TODO: should we use string or cstring?
using namespace std;

class House
{
public:
	string name;
	string desc;
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
};

#endif
