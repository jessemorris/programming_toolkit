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

    /**
     * @brief Get the Log Level object
     * 
     * @param level const Level&
     * @return const std::string 
     */
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

    /**
     * @brief Get the Log Colour object from a Level type
     * 
     * @param level const Level&
     * @return const TerminalColourCode 
     */
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

    /**
     * @brief Get the Background Colour number
     * 
     * @return const TerminalColourCode 
     */
    inline const TerminalColourCode getBackgroundColour() {
        return TerminalColourCode::FG_DEFAULT;
    }
    
    /**
     * @brief Wrapper to correctly format the output string with a set colour
     * 
     */
    class ColourModifier {
        
        TerminalColourCode code;
        public:
            ColourModifier(const TerminalColourCode& code_) : code(code_) {}

            /**
             * @brief Gets the correct ColourModifier based on an input log level.
             * 
             * @param level const Level&
             * @return const ColourModifier 
             */
            static const ColourModifier fromLogLevel(const Level& level) {
                return ColourModifier(getLogColour(level));
            }

            /**
             * @brief Getd the background colour modified.
             * 
             * @return const ColourModifier 
             */
            static const ColourModifier resetColour() {
                return ColourModifier(getBackgroundColour());
            }

            /**
             * @brief Writes the colour string to terminal.
             * 
             * @param os 
             * @param mod 
             * @return std::ostream& 
             */
            friend std::ostream& operator<<(std::ostream& os, const ColourModifier& mod) {
                return os << "\033[" << mod.code << "m";
            }
    };

    /**
     * @brief Controls the format of a logging string based on the level of the message.
     * 
     */
    class Formatter {

        public:
            /**
             * @brief Construct a new Formatter object.
             * 
             * @param level_ Log level.
             * @param file_  The file the message was invoked from.
             * @param line_  The line the message was invoked from.
             * @param message_ The actual message.
             */
            Formatter(const Level& level_, 
                      const std::string& file_,
                      int line_,
                      const std::string& message_)
                :   level(level_),
                    file(file_),
                    line(line_),
                    message(message_) {}

            /**
             * @brief ostream overloader.
             * 
             * @param os 
             * @param f 
             * @return std::ostream& 
             */
            friend std::ostream& operator<<(std::ostream& os, const Formatter& f) {
                os << f.toString();
                return os;
            }

            /**
             * @brief Writes the formatter object to a string with the desired format.
             * 
             * @return const std::string 
             */
            const std::string toString() const {
                std::stringstream ss;
                ss << "[" << getLogLevel(level) << "] ["
                   << extractFile() << ": " << line << "] " << message;
                std::string log =  ss.str();
                return colouriseMessage(log);
            }
            

            /**
             * @brief Creates a formatter object specifically for the excpetion case
             * 
             * @param exceptionType Name of the exception class that is invoked.
             * @param file_ The file the exception was invoked from.
             * @param line_ The line the exception was invoked from.
             * @param message The exception message.
             * @return const Formatter 
             */
            static const Formatter formatException(const std::string& exceptionType, 
                                                     const std::string& file_,
                                                     int line_,
                                                     const std::string& message) {
                return Formatter(Level::EXCEPTION, file_, line_, message);
                
            }

        private:

            /**
             * @brief Colourises a message based on the formatter's log level.
             * 
             * @param message 
             * @return const std::string 
             */
            const std::string colouriseMessage(const std::string& message) const {
                std::stringstream ss;
                ss << ptk::logging::ColourModifier::fromLogLevel(level) 
                   << message 
                   << ptk::logging::ColourModifier::resetColour();
                return ss.str();
            }

            /**
             * @brief Formats the __FILE__ marcro to online the actual file the message was called.
             * 
             * The __FILE__ macro includes the full file path which can be annoying to visualise.
             * 
             * @return const std::string 
             */
            const std::string extractFile() const {
                return file.substr(file.rfind("/") + 1);
            }

        private:
            const Level level;
            const std::string file;
            const int line;
            const std::string message;
    };

    /**
     * @brief Throws an exception using the custom Formatter to display the message.
     * 
     * @tparam Exception_ An exception that inherits from std::runtime_exception
     * @param exceptionType 
     * @param file 
     * @param line 
     * @param message 
     */
    template<typename Exception_>
    inline void throwException(const std::string& exceptionType,
                           const std::string& file,
                           int line,
                           const std::string& message) {
        throw(Exception_(ptk::logging::Formatter::formatException(exceptionType, file, line, message).toString()));
    }



} //namesapce logging
} //namespace ptk