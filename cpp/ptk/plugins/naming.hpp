#ifndef PTK_PLUGINS_NAMING
#define PTK_PLUGINS_NAMING

#include "ptk/utilities/macros.h"

#include <string>


namespace ptk {
namespace plugins {

/**
 * @brief Manages the naming of a plugin class attributes based on the deriving class and base plugin class.
 * 
 * This is used to inject the runtime plugin instance object with its base name and actual name as well as set
 * thte allocation/deallocation class function name to actually load the correct object. 
 * 
 */
class PluginClassDetails {

    public:
        PTK_POINTER_TYPEDEFS(PluginClassDetails);

        PluginClassDetails(const std::string& derivedName_, const std::string& baseName_);
        //eveutally in the form namespace(?)::Class::BaseClass
        PluginClassDetails(const std::string& packageName);

        const std::string& getDerivedName() const;
        const std::string& getBaseName() const;

        const std::string& getAllocClassFuncName() const;
        const std::string& getDeleteClassFuncName() const;
        

    private:
        const std::string derivedName;
        const std::string baseName;

        std::string allocClassSymbol;
        std::string deleterClassSymbol;

        //TODO: replace in pluginloader eventually
        const std::string allocClassSymbolBase = "allocator";
        const std::string deleteClassSymbolBase = "deleter";

};


/**
 * @brief Manages the construction of a library path given the base lib path.
 * 
 * The base lib path should be of the form lib<baseLib>.so. The class will then use the packages module
 * to check along the deisred runpaths to find this library and construct the full name so the dynamic loader
 * class can open the library. 
 * 
 */
class DynamicLibraryPath {

    public:
        DynamicLibraryPath(const std::string& baseLib_);


        const std::string& getFullLibraryPath() const;
        const char* getFullLibraryPathCStr() const;

        const std::string& getBaseLibName() const;

        bool isPathValid() const;

    private:
        bool constructFullLibPath();
        bool validateFullLibPath();

        bool isFound;
        std::string baseLib;
        std::string fullLibPath;
};

} //plugins
} //ptk

#endif