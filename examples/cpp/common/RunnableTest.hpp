#ifndef PTK_EXAMPLES_COMMON_RUNNABLE_TEST
#define PTK_EXAMPLES_COMMON_RUNNABLE_TEST


#include "ptk/common/RunnableModule.hpp"
#include "ptk/utilities/logging.h"

using namespace ptk::common;

class RunnableAddFunction : public RunnableModule<int, int> {

    public:
        RunnableAddFunction() {
            registerOnCompletitionCallback(&RunnableAddFunction::testOnExecuteCallback, this);
        }

        RunnableModule::OutputSharedPtr execute(const int& input) override {
            return std::make_shared<int>(input + 10);
        }

        void testOnExecuteCallback(const std::shared_ptr<const int> output) {
            PTK_INFO_MSG("test on execute call back recieved with data: " << *output);
        }
};

#endif