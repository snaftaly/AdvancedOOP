#ifndef ALGOSCOMMON_H_
#define ALGOSCOMMON_H_

// typedef to make it easier to set up our factory
typedef AbstractAlgorithm * maker_t();
// our global factory
extern std::map<std::string, maker_t *, std::less<std::string> > factory;

#endif /* ALGOSCOMMON_H_ */
