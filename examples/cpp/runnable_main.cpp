#include <iostream>
#include <string>

#include "ptk/utilities/logging.h"
#include "common/RunnableTest.hpp"

using namespace ptk;

void externalCallback(const std::shared_ptr<const int> output) {
    PTK_INFO_MSG("In external callback with data: " << *output);
}

int main() {

    RunnableAddFunction runnable;
    runnable.registerOnCompletitionCallback(&externalCallback);
    RunnableResult<int, int> ressult = runnable.run(4);
    ressult.logDetails();
}