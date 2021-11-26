#ifndef PTK_COMMON_RUNNABLE_RESULT
#define PTK_COMMON_RUNNABLE_RESULT

#include "ptk/utilities/macros.h"
#include "ptk/utilities/logging.h"
#include "ptk/common/Time.hpp"

#include <functional>
#include <vector>

namespace ptk {
namespace common {

/**
 * @brief Contains the result of running an Runnable Module once. This includes the input used to run the module,
 * the actual result of executing the module and the runtime of the module. TODO: any more details. 
 * 
 * @tparam Input 
 * @tparam Output 
 */
template <typename Input, typename Output>
class RunnableResult {

    public:
        PTK_POINTER_TYPEDEFS(RunnableResult);

        RunnableResult(
            const std::shared_ptr<const Input> input_,
            const std::shared_ptr<const Output> output_,
            const Time& runtime_)
                :   input(input_),
                    result(output_),
                    runtime(runtime_) {}

            const std::shared_ptr<const Input> input;
            const std::shared_ptr<const Output> result;
            const Time runtime;

            /**
             * @brief Prints all the details of the result to stdin. The restuls include the input, output 
             * and runtime.
             * 
             */
            void printDetails() const {
                PTK_STDOUT(formatAllDetails());
            }

            /**
             * @brief Prints only the result of the runnable to stdin.
             * 
             */
            void printResult() const {
                PTK_STDOUT(formatResults());
            }

            /**
             * @brief Prints all the details using the PTK_INFO_MSG macro. The restuls include the input, output 
             * and runtime.
             * 
             */
            void logDetails() const {
                PTK_INFO_MSG(formatAllDetails());
            }

            /**
             * @brief Prints only the result using the PTK_INFO_MSG macro. 
             * 
             */
            void logResult() const  {
                PTK_INFO_MSG(formatResults());
            }

        private:
            std::string formatAllDetails() const {
                std::stringstream ss;
                ss << "\nResult: " << *result << std::endl;
                ss << "Input: " << *input << std::endl;
                ss << "Runtime: " << runtime;
                return ss.str();
            } 

            std::string formatResults() const {
                std::stringstream ss;
                ss << "\nResult: " << *result; 
                return ss.str();
            }
};



} //common
} //ptk


#endif
