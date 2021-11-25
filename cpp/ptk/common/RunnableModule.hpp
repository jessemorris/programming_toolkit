#ifndef PTK_COMMON_RUNNABLE_MODULE
#define PTK_COMMON_RUNNABLE_MODULE

#include "ptk/utilities/macros.h"
#include "ptk/utilities/logging.h"
#include "ptk/common/Time.hpp"

namespace ptk {
namespace common {

struct RunnableOptions {
    PTK_POINTER_TYPEDEFS(RunnableOptions);

    bool runForeground = false;
    //TODO

};


template <typename Input, typename Output>
class RunnableModule {

    public:
        PTK_POINTER_TYPEDEFS(RunnableModule);
        PTK_DELETE_COPY_CONSTRUCTORS(RunnableModule);

        using InputSharedPtr = std::shared_ptr<Input>;
        using OutputSharedPtr = std::shared_ptr<Output>;

        RunnableModule(RunnableOptions::UniquePtr options_)
            :   options(options_),
                lastInput(nullptr),
                lastOutput(nullptr) {}

        //this one will just use the detault options
        RunnableModule()
            :   options(std::make_unique<RunnableOptions>()),
                lastInput(nullptr),
                lastOutput(nullptr) {}


        OutputSharedPtr run(InputSharedPtr input) {
            //TODO: time algorithm
            OutputSharedPtr result = execute(input);


            lastInput = input; //actually these should be copies so we can modify them after return. Or maybe input/ouput are turn by value?
            lastOutput = result;

            return result;
        }

        void printRunnableOptions() const {

        }

        const Time getExecutionDuration() const {

        }

        const RunnableOptions* getOptions() {
            return options.get();
        }

        const OutputSharedPtr getLastOutput() const  {
            return lastOutput;
        }

        const InputSharedPtr getLastInput() const {
            return lastInput;
        }


    protected:
        /**
         * @brief The actual function that executes the algorithm.
         * 
         * @param input InputSharedPtr
         * @return OutputSharedPtr 
         */
        OutputSharedPtr execute(InputSharedPtr input) = 0;

    private:
        //TODO: add run in thread etc?
        //TODO: collect stats 


        RunnableOptions::UniquePtr options;
        OutputSharedPtr lastOutput;
        InputSharedPtr lastInput;
};



} //common
} //ptk


#endif