#ifndef PTK_PLUGINS_NAMING
#define PTK_PLUGINS_NAMING

#include "ptk/utilities/macros.h"

#include <string>


namespace ptk {
namespace plugins {

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


//TODO: some namespace
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