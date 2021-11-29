#ifndef PTK_UTILITIES_OUTPUTWRITER
#define PTK_UTILITIES_OUTPUTWRITER

#include "ptk/utilities/macros.h"
#include "ptk/utilities/logging-common.h"

#include <string>
#include <map>
#include <mutex>

#include <iostream>
#include <fstream>
#include <sstream>


namespace ptk {

namespace utils {


// struct HandlerConfiguration {

//     logging::Level level =  logging::Level::INFO;
//     std::string fileName = "";
    
// };


namespace impl {


class OutputHandler {
    public:
        PTK_POINTER_TYPEDEFS(OutputHandler);

        using HandlerName = std::string;
        using OutputHandlerMap = std::map<HandlerName, OutputHandler::Ptr>;

        /**
         * @brief Get the Handler object
         * 
         * @tparam Handlder Must derive from OutputHandler
         * @param name 
         * @return std::shared_ptr<const HandlerName> 
         */
        template<class Handlder>
        static OutputHandler::Ptr getHandler(const std::string& name) {
                if (handlerMap.find(name) == handlerMap.end()){
                OutputHandler::Ptr ptr = std::make_shared<Handlder>(name); 
                handlerMap.emplace(name, ptr);
                return ptr;
            }
            else {
                return handlerMap.at(name);
            }
        }

        template<class T>
        void log(const T& record);


        template<class T>
        void log(const T* record, const logging::Level& level);


    private:

        static OutputHandlerMap handlerMap;

    protected:
        explicit OutputHandler(const std::string& name_);
        virtual void handle(const std::string& log) = 0;
        
        const std::string name;
        std::mutex mutex;
        // HandlerConfiguration config;

};


template<class T>
inline void OutputHandler::log(const T& record) {
    mutex.lock();
    std::stringstream ss;
    ss << record;
    handle(ss.str());
    mutex.unlock();
}




//probably shouldnt be impl?
class FileHandler {
    public:
        //maybe put in another file
        enum FileType {
            TXT,
            YAML
        };


        FileHandler(const std::string& name_);
        ~FileHandler();

        inline const std::string& getFilePath() const {return filePath; }
        
        template<class T>
        void write(const T& data);

    private:
        const std::string constructOutputFilePath(const std::string& file_) const;
        const std::string filePath;
        const FileType fileExtension = FileType::TXT;

    protected:
        std::ofstream file;

};

template<>
inline void FileHandler::write(const std::string& data) {
    std::cout << "writing " << data << " to " << getFilePath() << std::endl;
    file << data << "\n";
}

class LoggingHandler : public OutputHandler {
    public:
        LoggingHandler(const std::string& name_);

    protected:
        virtual void handle(const std::string& log) override;
};




} //impl
} //utils
} //ptk


#endif