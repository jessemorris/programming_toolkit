#ifndef PTK_PLUGINS_DETAILS
#define PTK_PLUGINS_DETAILS


#include <string>

namespace ptk {
namespace plugins {

template<class T>
class DynamicLoader;

class PluginBase;
class DynamicLibraryPath;

//TODO: a better name might be reflectionDetails 
class PluginBaseDetails {

    public:
        PluginBaseDetails(
            const std::string& derivedName_,
            const std::string& baseName_,
            const DynamicLibraryPath* libraryPath_);

        //TODO: should all be const
        const std::string derivedName;
        const std::string baseName;
        const DynamicLibraryPath* libraryPath;
        DynamicLoader<PluginBase>* loaderPtr;

};

} //plugins
} //ptk

#endif
