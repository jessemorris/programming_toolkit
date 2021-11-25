#ifndef PTK_COMMON_ALGORITHM
#define PTK_COMMON_ALGORITHM

#include "ptk/common/RunnableModule.hpp"

#include "ptk/utilities/macros.h"
#include "ptk/utilities/logging.h"



namespace ptk {
namespace common {


struct AlgorithmOptions : public RunnableOptions {
    

};

//inspried by opencv's algoritm class -> to add improvements
class Algorithm {

    public:
        PTK_POINTER_TYPEDEFS(Algorithm);

        //Options here -> plus no options constructor
        Algorithm() = default;
        virtual ~Algorithm() = default;

        /**
         * @brief Clears the state of the algorithm.
         * 
         */
        virtual void clearState() = 0;

        //TODO:: read/write as state etc




};


template <typename Input, typename Output>
class AlgorithmModule : public Algorithm, public RunnableModule<Input, Output>  {

    public:
        PTK_POINTER_TYPEDEFS(AlgorithmModule);
        PTK_DELETE_COPY_CONSTRUCTORS(AlgorithmModule);

        using RM = RunnableModule<Input, Output>;
       

    protected:
        virtual void clearState() override {}


};

} //common
} //ptk

#endif