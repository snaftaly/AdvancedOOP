#ifndef __ABSTRACT_ALGORITHM__H_
#define __ABSTRACT_ALGORITHM__H_

#include <map>
using namespace std;

#include "AbstractSensor.h"

class AbstractAlgorithm 
{ 
public: 
	virtual ~AbstractAlgorithm() {}
	
    // setSensor is called once when the Algorithm is initialized 
    virtual void setSensor(const AbstractSensor& sensor) = 0; 
    
    // setConfiguration is called once when the Algorithm is initialized - see below 
    virtual void setConfiguration(map<string, int> config) = 0; 
    
    // step is called by the simulation for each time unit 
    virtual Direction step() = 0; 
    
    // this method is called by the simulation either when there is a winner or 
    // when steps == MaxSteps - MaxStepsAfterWinner 
    // parameter stepsTillFinishing == MaxStepsAfterWinner 
    virtual void aboutToFinish(int stepsTillFinishing) = 0; 
};

// TODO: we can't use this - how can we solve it???
// typedef to make it easier to set up our factory
typedef AbstractAlgorithm * maker_t();
// our global factory
extern std::map<std::string, maker_t *, std::less<std::string> > factory;


#endif //__ABSTRACT_ALGORITHM__H_
