/*
 * simpleAlgorithm.cpp
 *
 *  Created on: Mar 12, 2016
 *      Author: noaleibo1
 */

#include "simpleAlgorithm.h"

simpleAlgorithm::simpleAlgorithm() {
	// TODO Auto-generated constructor stub

}

simpleAlgorithm::~simpleAlgorithm() {
	// TODO Auto-generated destructor stub
}

virtual Direction step(){
	int i = rand() % 4;
	return Direction[i];
}
