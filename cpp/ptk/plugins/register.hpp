#ifndef PTK_PLUGINS_REGISTER
#define PTK_PLUGINS_REGISTER

#include <memory>
#include <iostream>

#include "LibraryLoader.hpp"

#define PTK_REGISTER_CUSTOM_PLUGIN_LOADER(PluginType)   \
    using PluginType##Loader = DynamicLoader<PluginType>;    \
    using PluginType##Ptr = std::shared_ptr<PluginType>;

#define PTK_REGISTER_PLUGIN_CLASS_IMPL(Derived, Base, UniqueID) \
    namespace \
    { \
    struct ProxyExec ## UniqueID \
    { \
        typedef  Derived _derived; \
        typedef  Base _base; \
        ProxyExec ## UniqueID() \
        { \
        class_loader::registerPlugin<_derived, _base>(#Derived, #Base); \
        }   \
    }; \
    static ProxyExec ## UniqueID g_register_plugin_ ## UniqueID; \
    }  


#define PTK_REGISTER_PLUGIN_CLASS(Derived, Base) \
    extern "C"  { \
        Derived *allocator##Derived() {   \
            return new Derived();    \
        }   \
        void deleter##Derived(Derived *ptr) { \
            if(ptr != nullptr) { \
                delete ptr; \
            }   \
        }   \
    }   \
    PTK_REGISTER_PLUGIN_CLASS_IMPL(Derived, Base, __COUNTER__)

#endif