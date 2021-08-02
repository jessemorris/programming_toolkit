#pragma once

#include <ostream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <ostream>
#include <sstream>

//TODOS(jesse ) -> put in better namespacing so helper functions are more hidden from user and we only 
//expose "public" macros at the top namespace level

//define colour codes for logging
//https://stackoverflow.com/questions/2616906/how-do-i-output-coloured-text-to-a-linux-terminal
namespace ptk {
namespace logging {

    enum Level {
        INFO,
        DEBUG,
        WARN,
        ERROR,
        EXCEPTION
    };


    enum TerminalColourCode {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_YELLOW   = 33,
        FG_BLUE     = 34,
        FG_MAGENTA  = 35,
        FG_CYAN     = 36,
        FG_DEFAULT  = 39,
        FG_LIGHT_RED = 91,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49
    };

    inline const std::string getLogLevel(const Level& level) {
        switch(level) {
            case Level::DEBUG:
                return std::string{"DEBUG"};
            case Level::INFO:
                return std::string{"INFO"};
            case Level::WARN:
                return std::string{"WARN"};
            case Level::ERROR:
                return std::string{"ERROR"};
            case Level::EXCEPTION:
                return std::string{"EXCEPTION"};
            default:
                return std::string{"BG"}; //for background colour
        }
    }

    inline const TerminalColourCode getLogColour(const Level& level) {
        //i'm just picking random colours *shurgs*
        switch(level) {
            case Level::DEBUG:
                return TerminalColourCode::FG_GREEN;
            case Level::INFO:
                return TerminalColourCode::FG_DEFAULT;
            case Level::WARN:
                return TerminalColourCode::FG_YELLOW;
            case Level::ERROR:
                return TerminalColourCode::FG_RED;
            case Level::EXCEPTION:
                return TerminalColourCode::FG_CYAN;
            default:
                return TerminalColourCode::FG_DEFAULT; //for background colour
        }
    }

    inline const TerminalColourCode getBackgroundColour() {
        return TerminalColourCode::FG_DEFAULT;
    }
    
    class ColourModifier {
        
        TerminalColourCode code;
        public:
            ColourModifier(const TerminalColourCode& code_) : code(code_) {}

            //i'm just picking random colours *shurgs*
            static const ColourModifier fromLogLevel(const Level& level) {
                return ColourModifier(getLogColour(level));
            }

            static const ColourModifier resetColour() {
                return ColourModifier(getBackgroundColour());
            }

            friend std::ostream& operator<<(std::ostream& os, const ColourModifier& mod) {
                return os << "\033[" << mod.code << "m";
            }
    };

    class Formatter {

        public:
            Formatter(const Level& level_, 
                      const std::string& file_,
                      int line_,
                      const std::string& message_)
                :   level(level_),
                    file(file_),
                    line(line_),
                    message(message_) {}

            friend std::ostream& operator<<(std::ostream& os, const Formatter& f) {
                os << f.toString();
                return os;
            }

            const std::string toString() const {
                std::stringstream ss;
                ss << "[" << getLogLevel(level) << "] ["
                   << extractFile() << ": " << line << "] " << message;
                std::string log =  ss.str();
                return colouriseMessage(log);
            }
            

            static const Formatter formatException(const std::string& exceptionType, 
                                                     const std::string& file_,
                                                     int line_,
                                                     const std::string& message) {
                return Formatter(Level::EXCEPTION, file_, line_, message);
                
            }

        private:

            const std::string colouriseMessage(const std::string& message) const {
                std::stringstream ss;
                ss << ptk::logging::ColourModifier::fromLogLevel(level) 
                   << message 
                   << ptk::logging::ColourModifier::resetColour();
                return ss.str();
            }

            const std::string extractFile() const {
                return file.substr(file.rfind("/") + 1);
            }

        private:
            const Level level;
            const std::string file;
            const int line;
            const std::string message;
    };


    template<typename Exception_>
    inline void throwException(const std::string& exceptionType,
                           const std::string& file,
                           int line,
                           const std::string& message) {
        throw(Exception_(ptk::logging::Formatter::formatException(exceptionType, file, line, message).toString()));
    }



} //namesapce logging
} //namespace ptk