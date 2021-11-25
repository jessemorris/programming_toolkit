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
    // PipelinePluginPtr special = dlloader.load("Tatooine");
    // // PipelinePluginPtr pipeline = dlloader.load("Pipeline");

	// // planet->onInit();
    // // planet->spinOnce();

    // std::cout << special->getLibraryPath() <<std::endl;
    // std::cout << special->getName() <<std::endl;
    // std::cout << special->getPluginBaseName() <<std::endl;

    // std::cout << "Unloading " << path << std::endl;
	// dlloader.DLCloseLib();


}