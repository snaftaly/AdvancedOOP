#include <functional>
#include <memory>

#include "../Common/Direction.h"
#include "../Common/AbstractAlgorithm.h"
#include "../Common/AlgorithmRegistration.h"
#include "../Common/MakeUniqueAdder.h"
#include "AlgorithmRegistrar.h"

using namespace std;

AlgorithmRegistration::AlgorithmRegistration(std::function<std::unique_ptr<AbstractAlgorithm>()> algorithmFactory) {
    AlgorithmRegistrar::getInstance().registerAlgorithm(algorithmFactory);
}
