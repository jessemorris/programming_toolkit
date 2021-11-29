#include "ptk/utilities/outputHandler.hpp"

#include <iostream>

namespace ptk {
namespace utils {
namespace impl {

LoggingHandler::LoggingHandler(const std::string& name_)
    :   OutputHandler(name_) {}

void LoggingHandler::handle(const std::string& log) {
    std::cout << log << std::endl;
}


} //impl
} //utils
} //ptk