/*
 * AlgorithmRunner.cpp
 *
 *  Created on: Mar 17, 2016
 *      Author: noaleibo1
 */

#include "AlgorithmRunner.h"

AlgorithmRunner::AlgorithmRunner(AbstractAlgorithm* a): houseTotalDirtLevel(-1), isRunning(true) {
	algorithm = a;
	//TODO refer to battery.
}

AlgorithmRunner::~AlgorithmRunner() {
	// TODO Auto-generated destructor stub
}


