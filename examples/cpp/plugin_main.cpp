#include <iostream>
#include <string>

#include "ptk/utilities/logging.h"
#include "ptk/plugins/LibraryLoader.hpp"
#include "plugins/testPlugin.hpp"

using namespace ptk;

int main() {
    // std::string path = "/home/jesse/Code/src/programming_toolkit/build/lib";
    TestLoader loader("plugin_example"); 
    // plugins::P dlloader("tatooinePlugin");
    PTK_INFO_MSG("Running plugin main");
    TestPtr test = loader.load("Test");
    PTK_INFO_MSG("here");
    //TODO: without this the program seg faults becuase I dont think the constructor on the 
    //static lib (which contains shared ptr's) is called in the same way. Need better way of doing this
    plugins::library_loader::clearNamedMap();



}