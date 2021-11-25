#ifndef PTK_PLUGINS_PLUGIN
#define PTK_PLUGINS_PLUGIN

#include "details.hpp"
#include "register.hpp"


namespace ptk {
namespace plugins {

/*
** This is the base class for all planets (libraries)
*/
class PluginBase {


    public:
        virtual ~PluginBase() = default;

       /**
        * @brief Initalise method to set any variables required by the base class.
        * 
        * Constructor must be empty (for plugin trickery) so this class is called by the loader immeidately
        * after memory allocation.
        * 
        */
        virtual void onInit() = 0;


        //TODO: should be private and make dynamicLoader friend?
        /**
         * @brief Set the Details of this object including its own name, plugin base, library it was laoded from
         * and the loader used (currently null). Called internally by the loader and should be be called publically. 
         * 
         * @param details_ 
         */
        void setDetails(std::unique_ptr<PluginBaseDetails> details_);

        /**
         * @brief Get the name of the deriving class as a string.
         * 
         * @return const std::string& 
         */
        const std::string& getName() const;

        /**
         * @brief Get the name of the base plugin class as a string.
         * 
         * @return const std::string& 
         */
        const std::string& getPluginBaseName() const;

        /**
         * @brief Get the path to the dynamic library this class was loaded from as a string
         * 
         * @return const std::string& 
         */
        const std::string& getLibraryPath() const;

        /**
         * @brief Get a pointer to the loader object used to instantiate this clas. Currently always null.
         * 
         * @return const DynamicLoader<PluginBase>* 
         */
        const DynamicLoader<PluginBase>* getLoader() const;

    private:
        std::unique_ptr<PluginBaseDetails> details;

};

} //plugins
} //ptk

#endif