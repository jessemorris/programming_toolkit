#ifndef PTK_CORE_IMPL_LOGGER
#define PTK_CORE_IMPL_LOGGER


#include "ptk/utilities/outputHandler.hpp"
#include "ptk/utilities/macros.h"

namespace ptk {
namespace utils {
namespace impl {


static const std::string getInternalLoggerName();

class InternalLogger : public OutputHandler {
    public:
        PTK_POINTER_TYPEDEFS(InternalLogger);
    
        InternalLogger(const std::string& name_);
    protected:
        virtual void handle(const std::string& log) override;
    private:
        FileHandler fileHandler;
};

OutputHandler::Ptr getInternalLogger();

} //impl
} //utils
} //ptk




#endif