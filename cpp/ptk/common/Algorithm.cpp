#include "ptk/common/Algorithm.hpp"

namespace ptk {
namespace common {

Algorithm::Algorithm()
    :   options(std::make_unique<AlgorithmOptions>()) {}

Algorithm::Algorithm(AlgorithmOptions::UniquePtr options_)
    :   options(std::move(options_)) {}

const AlgorithmOptions* Algorithm::getAlgorithmOptions() const {
    return options.get();
}

} //common
} //ptk