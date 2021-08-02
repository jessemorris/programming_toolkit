#pragma once

#include "logging-common.h"

#define PTK_DEFINE_EXCEPTION(exceptionName) \
    class exceptionName : public std::runtime_error {   \
        public: \
            exceptionName(const char * message) : runtime_error(message) {}   \
            exceptionName(std::string const & message) : runtime_error(message) {} \
            virtual ~exceptionName() throw() {}                 \
    };



#define LOGGING_STREAM_BASE(level, x) \
    ptk::logging::Formatter(level, __FILE__, __LINE__, x)


#define PTK_LOG(level, x) \
    {   \
        std::stringstream ss;   \
        ss << x;                \
        switch(level){          \
            case ptk::logging::Level::DEBUG: { \
                std::cout << LOGGING_STREAM_BASE(ptk::logging::Level::DEBUG, ss.str()) << std::endl; \
                break;          \
            }                   \
            case ptk::logging::Level::INFO: { \
                std::cout << LOGGING_STREAM_BASE(ptk::logging::Level::INFO, ss.str()) << std::endl; \
                break;          \
            }                   \
            case ptk::logging::Level::WARN: { \
                std::cout << LOGGING_STREAM_BASE(ptk::logging::Level::WARN, ss.str()) << std::endl; \
                break;          \
            }                   \
            case ptk::logging::Level::ERROR: { \
                std::cout << LOGGING_STREAM_BASE(ptk::logging::Level::ERROR, ss.str()) << std::endl; \
                break;          \
            }                   \
        }                       \
    }                           

//actual output logging messages to standard out
#ifdef USE_VIZ_DEBUG
    #define PTK_DEBUG_MSG(x) \
        PTK_LOG(ptk::logging::Level::DEBUG, x);

#else
    #define PTK_DEBUG_MSG(x) noop
#endif

#define PTK_INFO_MSG(x) \
    PTK_LOG(ptk::logging::Level::INFO, x)
    
#define PTK_WARN_MSG(x) \
    PTK_LOG(ptk::logging::Level::WARN, x)

#define PTK_ERROR_MSG(x) \
    PTK_LOG(ptk::logging::Level::ERROR, x)


//compiler makes unreadeable error if we try to use an excpetionType that has not been delcared with
//LR_DEFINE_EXCEPTION. TODO(jesse): wrap in try catch to tell user to define that exception class
#define PTK_THROW_EXCEPTION(exceptionType, x) \
    {   \
        std::stringstream ss;   \
        ss << x;                \
        throwException<exceptionType>("#exceptionType", std::string(__FILE__), __LINE__, ss.str()); \
    }


PTK_DEFINE_EXCEPTION(IndexOutOfBounds);

