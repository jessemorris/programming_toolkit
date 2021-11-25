#include "ptk/plugins/LibraryLoader.hpp"

namespace ptk {
namespace plugins {
namespace library_loader {


PluginClassDetailsMap& getNamedMap() {
    static PluginClassDetailsMap instance;
    return instance;
}

const PluginClassDetails::Ptr getClassNaming(const std::string& derived) {
    PluginClassDetailsMap& map = getNamedMap();
    if (map.find(derived) == map.end()) {
            return nullptr;
    }
    else {
        return map.at(derived);
    }
}

void clearNamedMap() {
    PluginClassDetailsMap& map = getNamedMap();
    map.clear();
}

} //library_loader
} //plugins
} //ptk