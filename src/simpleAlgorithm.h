/*
 * simpleAlgorithm.h
 *
 *  Created on: Mar 12, 2016
 *      Author: noaleibo1
 */

#ifndef SIMPLEALGORITHM_H_
#define SIMPLEALGORITHM_H_

#include <string>
#include <map>
#include <iostream>
#include <cstdlib>

class simpleAlgorithm {
public:
	simpleAlgorithm();
	virtual ~simpleAlgorithm();
	virtual void setSensor(const Sensor& s) = 0{
	}
	virtual void setConfiguration(std::map<std::string, int> config) = 0{
	}
	virtual Direction step(){
	}



};

#endif /* SIMPLEALGORITHM_H_ */
