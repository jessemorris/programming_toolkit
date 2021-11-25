// #ifndef PTK_PLUGINS_LIBRARY_LOADER
// #define PTK_PLUGINS_LIBRARY_LOADER
#pragma once


#include "naming.hpp"
#include "details.hpp"

#include "ptk/utilities/logging.h"

#include <memory>
#include <string>

#include <iostream>
#include <dlfcn.h>
#include <map>

namespace ptk {
namespace plugins {

namespace library_loader {

// #if defined(_MSC_VER)
// #   define DLL_EXPORT __declspec(dllexport)
// #   define DLL_IMPORT __declspec(dllimport)
// #elif defined(__GNUC__)
// #   define DLL_EXPORT __attribute__((visibility("default")))
// #   define DLL_IMPORT
// #   if __GNUC__ > 4
// #       define DLL_LOCAL __attribute__((visibility("hidden")))
// #   else
// #       define DLL_LOCAL
// #   endif
// #else
// #   error("Don't know how to export shared object libraries")
// #endif


// #define LIBRARY_PUBLIC_EXPORT DLL_EXPORT
using PluginClassDetailsMap = std::map<std::string, PluginClassDetails::Ptr>;

/**
 * @brief Get the Named Map object which contains a set of details for each derived class name
 * 
 * @return PluginClassDetailsMap& 
 */
PluginClassDetailsMap& getNamedMap();

/**
 * @brief Get the Class Naming object given the derived class name as registered by REGISTER_PLUGIN_CLASS.
 * If the class has not been registered, this function will return null
 * 
 * @param derived 
 * @return const PluginClassDetails::Ptr 
 */
const PluginClassDetails::Ptr getClassNaming(const std::string& derived);

/**
 * @brief Imp function called by REGISTER_PLUGIN_CLASS
 * 
 * @tparam Derived Derived class type
 * @tparam Base Base class type. Should inherit from PluginBase
 * @param class_name 
 * @param base_class_name 
 */
template<typename Derived, typename Base>
void registerPlugin(const std::string & class_name, const std::string & base_class_name) {
    //TODO: mame impl log
    std::cout << "registering plugin with name " << class_name << " base type " << base_class_name << std::endl;
    PluginClassDetailsMap& map = getNamedMap();
    map.emplace(class_name, std::make_shared<PluginClassDetails>(class_name, base_class_name));


}

void clearNamedMap();


} //library_loader



template <class PluginType>
class ClassLoaderBase {

public:

    /*
    ** Return a shared pointer on an instance of class loaded through
    ** a dynamic library.
    */
    virtual std::shared_ptr<PluginType>	load(const std::string& derived) = 0;


protected:

    const std::string allocClassSymbolBase = "allocator";
    const std::string deleteClassSymbolBase = "deleter";

    using AllocClassFunc = PluginType *(*)();
    using DeleteClassFunc = void (*)(PluginType *);

    virtual ~ClassLoaderBase() = default;

    /*
    **
    */
    virtual void dynamicLibOpen() = 0;


    /*
    ** Correctly delete the instance of the "dynamically loaded" class.
    */
    virtual void dynamicLibClose() = 0;


};


template <class T>
class DynamicLoader : public ClassLoaderBase<T> {
    using CLB = ClassLoaderBase<T>;

public:

    DynamicLoader(const std::string &pathToLib)
        :   _handle(nullptr), 
            libPath(pathToLib)
    {
        dynamicLibOpen();
    }

    ~DynamicLoader() {
        dynamicLibClose();
    }


    std::shared_ptr<T> load(const std::string& derived) {

        auto& map1 = library_loader::getNamedMap();

        const PluginClassDetails::Ptr name = library_loader::getClassNaming(derived);
        if(!name) {
            std::cout << "PluginNames was null for " << derived << std::endl;
            std::cout << "This class does not exist" << std::endl;
            //TODO: this will be hit it the class does not exist
            return nullptr;
        }

        auto allocFunc = reinterpret_cast<typename CLB::AllocClassFunc>(
                dlsym(_handle, name->getAllocClassFuncName().c_str()));
        auto deleteFunc = reinterpret_cast<typename CLB::DeleteClassFunc>(
                dlsym(_handle, name->getDeleteClassFuncName().c_str()));

        if (!allocFunc || !deleteFunc) {
            dynamicLibClose();
            std::cerr << dlerror() << std::endl;
            return nullptr;
        }

        auto instance =  std::shared_ptr<T>(
                allocFunc(),
                [deleteFunc](T *p){ deleteFunc(p); });


        instance->setDetails(std::make_unique<PluginBaseDetails>(
            name->getDerivedName(),
            name->getBaseName(),
            &libPath
        ));

        instance->onInit();
        return instance;
    }



private:

    void dynamicLibOpen() override {
        if(!libPath.isPathValid()) {
            std::cout << "LibPath " << libPath.getFullLibraryPath() << " could not be opened becuase it does not exist" << std::endl;
            return;
        }

        if (!(_handle = dlopen(libPath.getFullLibraryPathCStr(), RTLD_NOW | RTLD_LAZY))) {
            std::cerr << dlerror() << std::endl;
        }
    }

    void dynamicLibClose() override {
        if (dlclose(_handle) != 0) {
            std::cerr << dlerror() << std::endl;
        }
        // PTK_INFO_MSG("Closing dyamic library " << libPath.getBaseLibName());
    }


    void* _handle;
    // const std::string _pathToLib;
    const DynamicLibraryPath libPath;


};

} //pluginss
} //ptk

// #endif
