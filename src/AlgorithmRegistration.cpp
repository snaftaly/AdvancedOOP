#include <functional>
#include <memory>

#include "AlgorithmRegistration.h"

using namespace std;

// TODO: change to unique ptr
AlgorithmRegistration::AlgorithmRegistration(std::function<std::shared_ptr<AbstractAlgorithm>()> algorithmFactory) {
    AlgorithmRegistrar::getInstance().registerAlgorithm(algorithmFactory);
}
