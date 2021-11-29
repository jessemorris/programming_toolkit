#include "fileSystem.hpp"


#include <dirent.h>
#include <limits.h>
#include <sys/stat.h>
#include <unistd.h>

namespace ptk {
namespace utils {

bool checkFileExists(const std::string& fileName) {
    struct stat info;
    if (stat(fileName.c_str(), &info) == -1) {
        return false;
    }
    return S_ISREG(info.st_mode);
}

bool checkDirectoryExists(const std::string &directory) {
    struct stat info;
    if (stat(directory.c_str(), &info) == -1) return false;
    return S_ISDIR(info.st_mode);
}

static const std::string getEnvVar(const std::string &envVar) {
    if (const char *env_p = std::getenv(envVar.c_str())) {
        return std::string(env_p);
    } 
    else {
        return "";
    }
}

const std::string getHomeDirectory() {
    std::string home_dir = getEnvVar("HOME");
    if (home_dir.empty() || !checkDirectoryExists(home_dir)) {
        home_dir = "/";
    }

    return home_dir;
}

} //utils

const std::string getDefaultPtkLoggingDir() {
    static const std::string folder = "/.ptk/logging";
    return utils::getHomeDirectory() + folder;
}


} //ptk
