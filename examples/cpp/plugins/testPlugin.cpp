#include "testPlugin.hpp"
#include "ptk/plugins/register.hpp"

#include <iostream>

void Test::onInit() {
    std::cout << "On Init called in testPlugin " << std::endl;
}


PTK_REGISTER_PLUGIN_CLASS(Test, PluginBase);
