#include "ptk/utilities/outputHandler.hpp"

namespace ptk {
namespace utils {
namespace impl {

OutputHandler::OutputHandlerMap OutputHandler::handlerMap{};



OutputHandler::OutputHandler(const std::string& name_)
    :   name(name_) {}


void OutputHandler::manageRecord(const Record* record) const {
    // mutex.lock();
    handle(record);
    // mutex.unlock();
}





} //impl
} //utils
} //ptk