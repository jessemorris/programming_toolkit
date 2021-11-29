#include "ptk/utilities/logging-common.h"

namespace ptk {
namespace logging {

LogRecord::LogRecord(const Level& level_, 
        const std::string& file_,
        int line_,
        const char* fmt_,
        ...)
    :   level(level_),
        file(extractFile(file_)),
        line(line_) {

        char* messageC = (char*)malloc(strlen(fmt_) * sizeof(char));
        va_list arglist;
        va_start( arglist, fmt_ );
        vsprintf(messageC, fmt_, arglist);
        va_end( arglist );
        
        message = std::string(messageC);
        free(messageC);

    }

LogRecord::LogRecord(const Level& level_, 
                     const std::string& file_,
                     int line_,
                     const std::string& message_)
    :   level(level_),
        file(extractFile(file_)),
        line(line_),
        message(message_) {}

const std::string LogRecord::colouriseMessage(const std::string& message) const {
    std::stringstream ss;
    ss << ptk::logging::ColourModifier::fromLogLevel(level) 
        << message 
        << ptk::logging::ColourModifier::resetColour();
    return ss.str();
}

const std::string LogRecord::extractFile(const std::string& filePath) {
    return filePath.substr(filePath.rfind("/") + 1);
}

} //logging
} //ptk