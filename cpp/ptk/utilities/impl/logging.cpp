#include "ptk/utilities/logging.h"
#include "ptk/utilities/outputHandler.hpp"


namespace ptk {
namespace utils {
namespace impl {
    OutputHandler::Ptr getLogger() {
        return OutputHandler::getHandler<LoggingHandler>("ptk-log");
    }

} //impl
} //utils
} //ptk