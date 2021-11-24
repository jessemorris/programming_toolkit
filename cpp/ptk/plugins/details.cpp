#include "details.hpp"

namespace ptk {
namespace plugins {

PluginBaseDetails::PluginBaseDetails(
        const std::string& derivedName_,
        const std::string& baseName_,
        const DynamicLibraryPath* libraryPath_)
    :   derivedName(derivedName_),
        baseName(baseName_),
        libraryPath(libraryPath_),
        loaderPtr(nullptr) {}

}
}
