#ifndef PTK_PLUGINS_PLUGIN
#define PTK_PLUGINS_PLUGIN

#include "details.hpp"
#include "register.hpp"


namespace ptk {
namespace plugins {

/*
** This is the base class for all planets (libraries)
*/
class PluginBase {


    public:
        virtual ~PluginBase() = default;

        /*
        ** Pure method which will be overrided.
        */
        virtual void onInit() = 0;


        //TODO: should be private and make dynamicLoader friend?
        void setDetails(std::unique_ptr<PluginBaseDetails> details_);

        const std::string& getName() const;
        const std::string& getPluginBaseName() const;
        const std::string& getLibraryPath() const;
        const DynamicLoader<PluginBase>* getLoader() const;

    private:
        std::unique_ptr<PluginBaseDetails> details;

};

} //plugins
} //ptk

#endif