#ifndef PTK_CORE_PACKAGE
#define PTK_CORE_PACKAGE

#include <string>

namespace ptk {
namespace core {

/**
 * @brief Constructs the full path to a library based on the library base name.
 * 
 * The baseName is defined as lib<baseName>.so
 *
 * The function will check in usr/local/lib, usr/local/lib/ptk_plugins (can we also check inthe build
 * folder of the actual program -> could use a name CMake define for this or use the ptk launch system)
 * 
 * @param baseName 
 * @return const std::string& 
 */
const std::string constructPackagePath(const std::string& baseName);

/**
 * @brief Checks that the baseName is of the form lib<baseName>.so
 * 
 * @param baseName 
 * @return true 
 * @return false 
 */
bool checkNameIsBaseLibName(const std::string& baseName);


} //core
} //ptk


#endif