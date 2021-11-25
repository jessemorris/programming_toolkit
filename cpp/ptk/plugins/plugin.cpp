#include "plugin.hpp"
#include "ptk/plugins/LibraryLoader.hpp"
namespace ptk {
namespace plugins {



void PluginBase::setDetails(std::unique_ptr<plugins::PluginBaseDetails> details_) {
    //assume that this always gets called in the plugin loader
    details = std::move(details_);

}


const std::string& PluginBase::getName() const {
    return details->derivedName;
}

const std::string& PluginBase::getPluginBaseName() const {
    return details->baseName;
}
const std::string& PluginBase::getLibraryPath() const {
    return details->libraryPath->getFullLibraryPath();
}
const plugins::DynamicLoader<PluginBase>* PluginBase::getLoader() const {
    return details->loaderPtr;
}

} //plugins
} //ptk