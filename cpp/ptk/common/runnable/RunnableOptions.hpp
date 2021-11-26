#ifndef PTK_COMMON_RUNNABLE_OPTIONS
#define PTK_COMMON_RUNNABLE_OPTIONS

#include "ptk/utilities/macros.h"

#include <functional>
#include <vector>

namespace ptk {
namespace common {

struct RunnableOptions {
    PTK_POINTER_TYPEDEFS(RunnableOptions);

    bool runBackground = false;
    //TODO

};

} //common
} //ptk

#endif