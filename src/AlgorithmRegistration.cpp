#include <functional>
#include <memory>

#include "Direction.h"
#include "AbstractAlgorithm.h"
#include "MakeUniqueAdder.h"
#include "AlgorithmRegistration.h"
#include "AlgorithmRegistrar.h"

using namespace std;

AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
    AlgorithmRegistrar::getInstance().registerAlgorithm(algorithmFactory);
}
