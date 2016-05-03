#ifndef ALGORITHMREGISTRATION_H_
#define ALGORITHMREGISTRATION_H_

//TODO: is it ok to add the IFNDEF???
// TODO: change back to make unique when what has to be done is understood
#include <functional>
#include <memory>

class AlgorithmRegistration {
public:
	AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()>);
};

#define REGISTER_ALGORITHM(class_name) \
AlgorithmRegistration register_me_##class_name \
([]{return make_unique<class_name>();} );

#endif /* ALGORITHMREGISTRATION_H_ */
