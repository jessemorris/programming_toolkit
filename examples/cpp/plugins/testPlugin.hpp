#pragma once

#include "ptk/plugins/plugin.hpp"
#include "ptk/plugins/register.hpp"

class Test : public ptk::plugins::PluginBase {

    public:

        Test() = default;
        ~Test() = default;

        void onInit() override;

};

PTK_REGISTER_CUSTOM_PLUGIN_LOADER(Test);
