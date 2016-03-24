#include <iostream>
#include <fstream>
#include <cstring>
#include <list>
using namespace std;


int main(int argc, char* argv[]) {

	// TODO: what should be done in case the arguments are not correct?
	// default location of files
	string housesDir = ".";
	string configDir = ".";

	// check number of arguments
	if (argc != 1 && argc != 3 && argc != 5){
		cout << "Error: number of arguments" << endl;
		return 1;
	}
	// parse the arguments of the program and check parameter names
	for (int i = 1; i < argc; i=i+2){
		if (!strcmp("-config", argv[i])){
			configDir = argv[i+1];
		}
		else if (!strcmp("-house_path", argv[i])){
			housesDir = argv[i+1];
		}
		else {
			cout << "Error: wrong parameter name" << endl;
			return 1;
		}
	}

	cout << "Bye" << endl;

}
