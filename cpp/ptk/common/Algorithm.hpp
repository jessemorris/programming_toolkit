#ifndef PTK_COMMON_ALGORITHM
#define PTK_COMMON_ALGORITHM

#include "ptk/common/runnable/RunnableModule.hpp"

#include "ptk/utilities/macros.h"
#include "ptk/utilities/logging.h"


#include <memory>



namespace ptk {
namespace common {


struct AlgorithmOptions {
    PTK_POINTER_TYPEDEFS(AlgorithmOptions);

};

//inspried by opencv's algoritm class -> to add improvements
class Algorithm {

    public:
        PTK_POINTER_TYPEDEFS(Algorithm);
        PTK_DELETE_COPY_CONSTRUCTORS(Algorithm);

        //Options here -> plus no options constructor
        //Make option const& instead?
        Algorithm();
        Algorithm(AlgorithmOptions::UniquePtr options_);
        virtual ~Algorithm() = default;

        /**
         * @brief Clears the state of the algorithm.
         * 
         */
        virtual void clearState() = 0;

        //TODO:: read/write as state etc
        const AlgorithmOptions* getAlgorithmOptions() const;

    protected:
        AlgorithmOptions::UniquePtr options;


};

template <typename Input, typename Output>
class AlgorithmModule : public Algorithm, public RunnableModule<Input, Output>  {

    public:
        PTK_POINTER_TYPEDEFS(AlgorithmModule);
        PTK_DELETE_COPY_CONSTRUCTORS(AlgorithmModule);

        using RM = RunnableModule<Input, Output>;
        /**
         * @brief Construct a new Algorithm Module object TODO:
         * 
         * @param algorithmOptions 
         * @param runnableOptions 
         */
        AlgorithmModule(AlgorithmOptions::UniquePtr algorithmOptions, RunnableOptions::UniquePtr runnableOptions) 
            :   Algorithm(algorithmOptions),
                RunnableModule<Input, Output>(runnableOptions) {}

        /**
         * @brief Construct a new Algorithm Module object TODO:
         * 
         * @param algorithmOptions 
         */
        AlgorithmModule(AlgorithmOptions::UniquePtr algorithmOptions) 
            : Algorithm(algorithmOptions) {}

        
        /**
         * @brief Construct a new Algorithm Module object TODO:
         * 
         * @param runnableOptions 
         */
        AlgorithmModule(RunnableOptions::UniquePtr runnableOptions)
            :   RunnableModule<Input, Output>(runnableOptions) {}

        /**
         * @brief Construct a new Algorithm Module object TODO:
         * 
         */
        AlgorithmModule() {}


       

    protected:
        virtual void clearState() override {}


};

} //common
} //ptk

#endif