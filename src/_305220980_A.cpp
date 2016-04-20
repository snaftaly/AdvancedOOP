#include "_305220980_A.h"

Direction _305220980_A::step() {
	// TODO: do some shuffling to not get stuck
	return getStep(possibleMoves);
}

extern "C" {
AbstractAlgorithm *maker(){
   return new _305220980_A;
}
class proxy {
public:
   proxy(){
      // register the maker with the factory
      factory["305220980_A_"] = maker;
   }
};
// our one instance of the proxy
proxy p;
}
