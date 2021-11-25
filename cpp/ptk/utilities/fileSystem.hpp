#include <string>

namespace ptk {
namespace utils {
    //TODO: make base path/file class
    //really should be called filePath
    /**
     * @brief Checks if the file specified by the given file path exists
     * 
     * @param fileName Full path to the file
     * @return true 
     * @return false 
     */
    bool checkFileExists(const std::string& fileName);

}
}
