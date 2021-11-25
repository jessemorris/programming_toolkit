#include "ptk/core/package.hpp"
#include "ptk/utilities/logging.h"
#include "ptk/utilities/fileSystem.hpp"

#include <vector>

namespace ptk {
namespace core {

static const std::vector<std::string> searchPackagePaths = {
    "/usr/local/lib",
    "/usr/local/lib/ptk_plugins"
};

const std::string constructPackagePath(const std::string& baseName) {
    //TODO: check that basename does not have prefix or suffix
    if(!checkNameIsBaseLibName(baseName)) {
        return "";
    }
    std::string libName = "lib" + baseName + ".so";
    for(const std::string& path : searchPackagePaths) {
        PTK_DEBUG_MSG("Searching " << path << "for " << libName);
        const std::string filePath = path + "/" + libName;
        if (utils::checkFileExists(filePath)) {
            return filePath;
        }

    }
    PTK_ERROR_MSG("Could not find package " << baseName);
    return "";
}

bool checkNameIsBaseLibName(const std::string& baseName) {
    static const std::string libPrefix = "lib";
    static const std::string soSuffix = ".so";

    bool prefix = libPrefix == baseName.substr(0, libPrefix.size());
    bool suffix = soSuffix == baseName.substr(baseName.size() - soSuffix.size(), soSuffix.size());

    if(prefix || suffix) {
        PTK_ERROR_MSG(baseName << "is an invalid lib base name. Should be in the form lib<basname>.so");
        return false;
    }
    return true;
}

} //core
} //ptk