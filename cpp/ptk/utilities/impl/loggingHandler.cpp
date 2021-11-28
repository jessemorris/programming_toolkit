#include "ptk/utilities/outputHandler.hpp"

#include <iostream>

namespace ptk {
namespace utils {
namespace impl {

LoggingHandler::LoggingHandler(const std::string& name_)
    : OutputHandler(name_) {}

void LoggingHandler::handle(const Record* record) const {
    std::cout << *record << std::endl;
}


} //impl
} //utils
} //ptk