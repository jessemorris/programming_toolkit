#pragma once

#include "logging-common.h"
#include "ptk/utilities/outputHandler.hpp"

namespace ptk {
namespace utils {
namespace impl {
    OutputHandler::ConstPtr getLogger();

} //impl
} //utils
} //ptk
#define PTK_DEFINE_EXCEPTION(exceptionName) \
    class exceptionName : public std::runtime_error {   \
        public: \
            exceptionName(const char * message) : runtime_error(message) {}   \
            exceptionName(std::string const & message) : runtime_error(message) {} \
            virtual ~exceptionName() throw() {}                 \
    };



#define LOGGING_STREAM_BASE(level, stream) \
    {   \
        const ptk::logging::LogRecord* log = new ptk::logging::LogRecord(level, __FILE__, __LINE__, stream); \
        ptk::utils::impl::getLogger()->manageRecord(log);  \
        delete log; \
    }
    

#define LOGGING_FORMAT_BASE(level, ...) \
    {   \
        const ptk::logging::LogRecord*  log = new ptk::logging::LogRecord(level, __FILE__, __LINE__, __VA_ARGS__);    \
        ptk::utils::impl::getLogger()->manageRecord(log);  \
        delete log; \
    }
    

#define PTK_LOG_STREAM(level, stream) \
    {   \
        std::stringstream ss;   \
        ss << stream;                \
        LOGGING_STREAM_BASE(level, ss.str()); \
    }  


#define PTK_LOG_FORMAT(level, ...) \
    {   \
        LOGGING_FORMAT_BASE(level, __VA_ARGS__); \
    } 

#define noop ({;})

//actual output logging messages to standard out
#ifdef USE_VIZ_DEBUG
    #define PTK_DEBUG_MSG_F(...) \
        PTK_LOG_FORMAT(ptk::logging::Level::DEBUG, __VA_ARGS__);

    #define PTK_DEBUG_MSG(stream) \
        PTK_LOG_STREAM(ptk::logging::Level::DEBUG, stream);

#else
    #define PTK_DEBUG_MSG_F(...) noop
    #define PTK_DEBUG_MSG(stream) noop
#endif


//STREAMS
#define PTK_INFO_MSG(stream) \
    PTK_LOG_STREAM(ptk::logging::Level::INFO, stream)
    
#define PTK_WARN_MSG(stream) \
    PTK_LOG_STREAM(ptk::logging::Level::WARN, stream)

#define PTK_ERROR_MSG(stream) \
    PTK_LOG_STREAM(ptk::logging::Level::ERROR, stream)



//FORMAT SPECIFIERS
#define PTK_INFO_MSG_F(...) \
    PTK_LOG_FORMAT(ptk::logging::Level::INFO, __VA_ARGS__)
    
#define PTK_WARN_MSG_F(...) \
    PTK_LOG_FORMAT(ptk::logging::Level::WARN, __VA_ARGS__)

#define PTK_ERROR_MSG_F(...) \
    PTK_LOG_FORMAT(ptk::logging::Level::ERROR, __VA_ARGS__)


//compiler makes unreadeable error if we try to use an excpetionType that has not been delcared with
//LR_DEFINE_EXCEPTION. TODO(jesse): wrap in try catch to tell user to define that exception class
#define PTK_THROW_EXCEPTION(exceptionType, ...) \
    {   \
        throwException<exceptionType>("#exceptionType", std::string(__FILE__), __LINE__, __VA_ARGS__); \
    }


PTK_DEFINE_EXCEPTION(IndexOutOfBounds); //TODO: example?

