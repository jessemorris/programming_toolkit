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

    /**
     * @brief Checks if the directory given the directory path exists
     * 
     * @param directory 
     * @return true 
     * @return false 
     */
    bool checkDirectoryExists(const std::string &directory);

    /**
     * @brief Gets the home directory of the system using PWD
     * 
     * @return const std::string 
     */
    const std::string getHomeDirectory();

    static const std::string getEnvVar(const std::string &envVar);

} //utils


const std::string getDefaultPtkLoggingDir();

} //ptk
