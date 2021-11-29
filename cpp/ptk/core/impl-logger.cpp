#include "ptk/core/impl-logger.hpp"
#include <iostream>

namespace ptk {
namespace utils {
namespace impl {

static const std::string getInternalLoggerName() {
    return "ptk-impl";
}

OutputHandler::Ptr getInternalLogger() {
    return OutputHandler::getHandler<InternalLogger>(getInternalLoggerName());
}

InternalLogger::InternalLogger(const std::string& name_ = getInternalLoggerName())
    :   OutputHandler(name_),
        fileHandler(name_) {}

void InternalLogger::handle(const std::string& log) {
    fileHandler.write(log);
    std::cout << log << std::endl;
}


} //impl
} //utils
} //ptk