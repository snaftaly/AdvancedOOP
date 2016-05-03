#include "_305220980_B.h"

Direction _305220980_B::step(Direction prevStep) {
	counter = (counter + 1) % 4;
	possibleMoves[0] = (Direction)counter;
	return getStepAndUpdatePrevStep(possibleMoves, prevStep);
}

//extern "C" {
//AbstractAlgorithm *maker(){
//   return new _305220980_B;
//}
//class proxy {
//public:
//   proxy(){
//      // register the maker with the factory
//      factory["305220980_B_"] = maker;
//   }
//};
//// our one instance of the proxy
//proxy p;
//}
REGISTER_ALGORITHM (_305220980_B)
