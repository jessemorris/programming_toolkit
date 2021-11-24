#pragma once

#include "ptk/plugins/plugin.hpp"


class Test : public ptk::plugins::PluginBase {

    public:

        Test() = default;
        ~Test() = default;

        void onInit() override;

};