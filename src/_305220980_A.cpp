#include "_305220980_A.h"

Direction _305220980_A::step() {
	counter = (counter + 1) % 10;
	if (counter == 1){
		Direction temp = possibleMoves[0];
		possibleMoves[0] = possibleMoves[3];
		possibleMoves[3] = temp;
	}
	if (counter == 6){
		Direction temp = possibleMoves[1];
		possibleMoves[1] = possibleMoves[2];
		possibleMoves[2] = temp;
	}
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
