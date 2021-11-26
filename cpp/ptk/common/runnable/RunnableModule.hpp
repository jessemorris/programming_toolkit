#ifndef PTK_COMMON_RUNNABLE_MODULE
#define PTK_COMMON_RUNNABLE_MODULE

#include "ptk/common/runnable/RunnableResult.hpp"
#include "ptk/common/runnable/RunnableOptions.hpp"

#include "ptk/utilities/macros.h"
#include "ptk/utilities/logging.h"
#include "ptk/common/Time.hpp"

#include <functional>
#include <vector>

namespace ptk {
namespace common {

/**
 * @brief A runnable module wraps the exeuction of a single virtual function that defines an input/output linear pipeline.
 * The virtual "exeucte" function is implemented by children classes and is run by the public "run" function. 
 * 
 * This base class is used to deine pipeline/algorithm modules to link different modules together and provide additional
 * information about a complex function including its runtime etc etc. Provides functionality for callbacks upon compleition
 * of the execute function and complex functions can be run in the background (TODO).
 * 
 * RunnableOptions is accepted as a constructor to change the way each module is run (TODO) and can be inherited by
 * deriving classes to provide specific options for each pipeline/algorithm.   
 * 
 * @tparam Input 
 * @tparam Output 
 */
template <typename Input, typename Output>
class RunnableModule {

    public:
        PTK_POINTER_TYPEDEFS(RunnableModule);
        PTK_DELETE_COPY_CONSTRUCTORS(RunnableModule);

        using InputSharedPtr = std::shared_ptr<Input>;
        using OutputConstSharedPtr = const std::shared_ptr<const Output>;
        using OutputSharedPtr = std::shared_ptr<Output>;


        RunnableModule(RunnableOptions::UniquePtr options_)
            :   options(options_),
                lastInput(nullptr),
                lastOutput(nullptr),
                count(0) {}

        //this one will just use the detault options
        RunnableModule()
            :   options(std::make_unique<RunnableOptions>()),
                lastInput(nullptr),
                lastOutput(nullptr),
                count(0) {}



        RunnableResult<Input, Output> run(const Input& input) {
            //TODO: add branching for run in foreground/background
            OutputSharedPtr output = execute(input);

            onExecuteCallbacks.execute(output);
            //actually these should be copies so we can modify them after return. Or maybe input/ouput are turn by value?
            lastInput = std::make_shared<Input>(input);
            lastOutput = output;
            executionCount++;

            //TODO: update with actual runtime.
            RunnableResult<Input, Output> result(
                std::const_pointer_cast<Input>(lastInput),
                std::const_pointer_cast<Output>(lastOutput),
                Time());
            return result;

        }

        void printRunnableOptions() const {
            //TODO
        }

        /**
         * @brief Get the duration of the last execution of this module
         * 
         * @return const Time 
         */
        const Time getExecutionDuration() const {
            //TODO
        }

        /**
         * @brief Get the options used to run this module. 
         * 
         * @return const RunnableOptions* 
         */
        const RunnableOptions* getOptions() {
            return options.get();
        }

        /**
         * @brief The number of times this module has been run
         * 
         * @return int 
         */
        int numberTimesRun() const {
            return executionCount;
        }

        /**
         * @brief Checks if the module has finished exeuction. Only useful if options.runBackground set to true
         * and the function is running asynchronously. Otherwise the functionn will always return true whe the module 
         * runs in the foreground.
         * 
         * @return true 
         * @return false 
         */
        bool isComplete() {
            if (options->runBackground) {
                return true;
            }
            else {
                return true;
            }
        }

        /**
         * @brief Get the Last Output object. Will be null of the moduel has not been run yet. 
         * 
         * @return const OutputSharedPtr 
         */
        const OutputSharedPtr getLastOutput() const  {
            return lastOutput;
        }

        const InputSharedPtr getLastInput() const {
            return lastInput;
        }

        /**
         * @brief Allows registration of a callback function immediately after the function is finished executing.
         * The function is given the result of the running the module. 
         * 
         * @tparam T 
         * @param fp pointer to the function to use as the callback
         * @param obj Pointer to the host object
         */
        template<class T>
        void registerOnCompletitionCallback(void(T::*fp)(OutputConstSharedPtr), T* obj) {
            onExecuteCallbacks.addCallback(fp, obj);
        }

        /**
         * @brief Allows registration of a callback function immediately after the function is finished executing.
         * The function is given the result of the running the module. 
         * 
         * @param fp 
         */
        void registerOnCompletitionCallback(void(*fp)(OutputConstSharedPtr)) {
            onExecuteCallbacks.addCallback(fp);
        }


    protected:
        /**
         * @brief The actual function that executes the algorithm.
         * 
         * @param input InputSharedPtr
         * @return OutputSharedPtr 
         */
        virtual OutputSharedPtr execute(const Input& input) = 0;

    private:

        class CallbackList {

            public:
                CallbackList() = default;
                ~CallbackList() = default;

                template<class T>
                void addCallback(void(T::*fp)(OutputConstSharedPtr), T* obj) {
                    std::function<void(OutputConstSharedPtr)> func = 
                        std::bind(fp, obj, std::placeholders::_1);
                    callbacks.push_back(func);
                }


                inline void addCallback(void(*fp)(OutputConstSharedPtr)) {
                    std::function<void(OutputConstSharedPtr)> func = fp;
                    callbacks.push_back(func);
                }

                inline void execute(OutputSharedPtr output) {
                    for(auto& func : callbacks) {
                        func(output);
                    }
                }

            private:
                std::vector<std::function<void(OutputConstSharedPtr)>> callbacks;
        };

        /**
         * @brief Runs the module in the current thread (blocking)
         * 
         * Note: this will cause the runnable result to be copied as we return it twice but this should be okay 
         * as we're only copying shared ptr's not the object!
         * @param input 
         * @return RunnableResult<Input, Output> 
         */
        RunnableResult<Input, Output> runInForeground(const Input& input) {
            return execute(input);
        }

        /**
         * @brief Runs the function in the background using std::future. Users should attach a callback to 
         * see when the function is done running or use the isComplete() function.
         * 
         * @param input 
         * @return RunnableResult<Input, Output> 
         */
        RunnableResult<Input, Output> runInBackground(const Input& input) {
            //TODO:
        }

        //TODO: collect stats 
        CallbackList onExecuteCallbacks;


        int executionCount;
        RunnableOptions::UniquePtr options;
        OutputSharedPtr lastOutput;
        InputSharedPtr lastInput;
};



} //common
} //ptk


#endif