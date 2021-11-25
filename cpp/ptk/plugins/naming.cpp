#include "ptk/plugins/naming.hpp"
#include "ptk/utilities/fileSystem.hpp"
#include "ptk/core/package.hpp"
#include "ptk/utilities/logging.h"

#include <iostream>


namespace ptk {
namespace plugins {

PluginClassDetails::PluginClassDetails(const std::string& derivedName_, const std::string& baseName_)
    :   derivedName(derivedName_),
        baseName(baseName_)
    {
        allocClassSymbol = allocClassSymbolBase + derivedName;
        deleterClassSymbol = deleteClassSymbolBase + derivedName;
    }


//TODO
PluginClassDetails::PluginClassDetails(const std::string& packageName) {}


const std::string& PluginClassDetails::getDerivedName() const {
    return derivedName;
}
const std::string& PluginClassDetails::getBaseName() const {
    return baseName;
}

const std::string& PluginClassDetails::getAllocClassFuncName() const {
    return allocClassSymbol;
}
const std::string& PluginClassDetails::getDeleteClassFuncName() const {
    return deleterClassSymbol;
}


DynamicLibraryPath::DynamicLibraryPath(const std::string& baseLib_)
        :   baseLib(baseLib_),
            isFound(false) {
        
        isFound = constructFullLibPath();
        PTK_INFO_MSG("Lib: " << fullLibPath << " found: " << isFound);

    }

const std::string& DynamicLibraryPath::getFullLibraryPath() const {
    return fullLibPath;
}

const char* DynamicLibraryPath::getFullLibraryPathCStr() const {
    return fullLibPath.c_str();
}

const std::string& DynamicLibraryPath::getBaseLibName() const {
    return baseLib;
}

bool DynamicLibraryPath::isPathValid() const {
    return isFound;
}

bool DynamicLibraryPath::constructFullLibPath() {
    //TODO::check forms etc
    fullLibPath = core::constructPackagePath(baseLib);
    return validateFullLibPath();
}

bool DynamicLibraryPath::validateFullLibPath() {
    if(fullLibPath.empty()) {
        return false;
        //TODO: add base logging
    }

    return utils::checkFileExists(fullLibPath);
}

} //plugins
} //ptk
