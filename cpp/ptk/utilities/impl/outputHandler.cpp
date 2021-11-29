#include "ptk/utilities/outputHandler.hpp"

namespace ptk {
namespace utils {
namespace impl {

OutputHandler::OutputHandlerMap OutputHandler::handlerMap{};

OutputHandler::OutputHandler(const std::string& name_)
    :   name(name_) {}






} //impl
} //utils
} //ptk