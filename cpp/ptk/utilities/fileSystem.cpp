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


} //utils
} //ptk
