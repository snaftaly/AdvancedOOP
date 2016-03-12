/*
 * Simulator.h
 *
 *  Created on: Mar 11, 2016
 *      Author: noaleibo1
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_

class Simulator {
	static House* houses;
	static simpleAlgorithm* algorithms;
	static Sensor* sensor;


public:
	Simulator(House* h, simpleAlgorithm* a, Sensor* s): houses(h), algorithms(a), sensor(s)
{
}
	virtual ~Simulator();
};

#endif /* SIMULATOR_H_ */
