#include "_305220980_C.h"

Direction _305220980_C::step() {
	return getStep(possibleMoves);
}

//extern "C" {
//AbstractAlgorithm *maker(){
//   return new _305220980_C;
//}
//class proxy {
//public:
//   proxy(){
//      // register the maker with the factory
//      factory["305220980_C_"] = maker;
//   }
//};
//// our one instance of the proxy
//proxy p;
//}

REGISTER_ALGORITHM (_305220980_C);
