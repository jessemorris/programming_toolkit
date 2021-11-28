#ifndef PTK_UTILITIES_OUTPUTWRITER
#define PTK_UTILITIES_OUTPUTWRITER

#include "ptk/utilities/macros.h"

#include <string>
#include <map>
#include "mutex"

namespace ptk {
namespace utils {

/**
 * @brief Base writeable that can be processed by the Output Handler classes
 * 
 */
class Record {
    public:
        Record() = default;
        virtual ~Record() = default;

        virtual const std::string toString() const = 0;

        /**
         * @brief ostream overloader. Calls virtual toString function. 
         * 
         * @param os 
         * @param f 
         * @return std::ostream& 
         */
        inline friend std::ostream& operator<<(std::ostream& os, const Record& record) {
            os << record.toString();
            return os;
        }
};


namespace impl {


class OutputHandler {
    public:
        PTK_POINTER_TYPEDEFS(OutputHandler);

        using HandlerName = std::string;
        using OutputHandlerMap = std::map<HandlerName, OutputHandler::ConstPtr>;

        /**
         * @brief Get the Handler object
         * 
         * @tparam Handlder Must derive from OutputHandler
         * @param name 
         * @return std::shared_ptr<const HandlerName> 
         */
        template<class Handlder>
        static OutputHandler::ConstPtr getHandler(const std::string& name) {
                if (handlerMap.find(name) == handlerMap.end()){
                OutputHandler::ConstPtr ptr = std::make_shared<const Handlder>(name); 
                handlerMap.emplace(name, ptr);
                return ptr;
            }
            else {
                return handlerMap.at(name);
            }
        }

        void manageRecord(const Record* record) const;


    private:

        static OutputHandlerMap handlerMap;

    protected:

        explicit OutputHandler(const std::string& name_);
        virtual void handle(const Record* record) const = 0;
        
        const std::string name;
        std::mutex mutex;

};

class LoggingHandler : public OutputHandler {
    public:
        LoggingHandler(const std::string& name_);

    protected:
        virtual void handle(const Record* record) const override;

};

class FileOutputHandler {

};

// class 

} //impl
} //utils
} //ptk


#endif