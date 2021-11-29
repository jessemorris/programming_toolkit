#pragma once

#include "macros.h"

#include <ostream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <string>
#include <ostream>
#include <sstream>

#include <stdarg.h>
#include <string.h>


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
                return TerminalColourCode::FG_CYAN;
            case Level::INFO:
                return TerminalColourCode::FG_DEFAULT;
            case Level::WARN:
                return TerminalColourCode::FG_YELLOW;
            case Level::ERROR:
                return TerminalColourCode::FG_RED;
            case Level::EXCEPTION:
                return TerminalColourCode::FG_RED;
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

    class LogRecord; //forward


    const std::string formatLogRecord(const LogRecord& record);
    const std::string colouriseRecord(const LogRecord& record);
    const std::string colouriseMessage(const std::string& message, const Level& level);


    /**
     * @brief Controls the recording of a logging string based on the level of the message.
     * 
     */
    class LogRecord : public Record {
        PTK_POINTER_TYPEDEFS(LogRecord);

        public:

            /**
             * @brief Construct a new Formatter for when formatting (printf) style object is needed.
             * 
             * @param level_ Log level.
             * @param file_  The file the message was invoked from.
             * @param line_  The line the message was invoked from.
             * @param fmt_ The message format.
             */
            LogRecord(const Level& level_, 
                      const std::string& file_,
                      int line_,
                      const char* fmt_,
                      ...);


            /**
             * @brief Construct a new Record object for when stream logging is used.
             * 
             * @param level_ Log level.
             * @param file_  The file the message was invoked from.
             * @param line_  The line the message was invoked from.
             * @param message_ The actual message.
             */
            LogRecord(const Level& level_, 
                      const std::string& file_,
                      int line_,
                      const std::string& message_);



            /**
             * @brief ostream overloader. Calls virtual onStreamInput function. 
             * 
             * @param os 
             * @param f 
             * @return std::ostream& 
             */
            friend std::ostream& operator<<(std::ostream& os, const LogRecord& record) {
                os << colouriseRecord(record);
                return os;
            }

            inline const Level& getLevel() const {return level; }
            inline const std::string& getFile() const {return file;}
            inline const int getLine() const {return line; }
            inline const std::string& getMessage() const {return message; }

            /**
             * @brief Writes the formatter object to a string with the desired format.
             * 
             * @return const std::string 
             */
            const std::string toString() const override {
                return formatLogRecord(*this);
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
            static const LogRecord formatException(const std::string& exceptionType, 
                                                   const std::string& file_,
                                                   int line_,
                                                   const std::string& message) {
                return LogRecord(Level::EXCEPTION, file_, line_, message);
                
            }

        private:

            /**
             * @brief Colourises a message based on the formatter's log level.
             * 
             * @param message 
             * @return const std::string 
             */
            const std::string colouriseMessage(const std::string& message) const;

            /**
             * @brief Formats the __FILE__ marcro to online the actual file the message was called.
             * 
             * The __FILE__ macro includes the full file path which can be annoying to visualise.
             * 
             * @return const std::string 
             */
            static const std::string extractFile(const std::string& filePath);

        private:
            const Level level;
            const std::string file;
            const int line;
            std::string message;
    };



    inline const std::string formatLogRecord(const LogRecord& record) {
        std::stringstream ss;
        ss << "[" << getLogLevel(record.getLevel()) << "] ["
            << record.getFile() << ": " << record.getLine() << "] " << record.getMessage();
        return ss.str();
    }

    inline const std::string colouriseRecord(const LogRecord& record) {
        return colouriseMessage(formatLogRecord(record), record.getLevel());
    }

    inline const std::string colouriseMessage(const std::string& message, const Level& level) {
        std::stringstream ss;
        ss << ptk::logging::ColourModifier::fromLogLevel(level) 
            << message 
            << ptk::logging::ColourModifier::resetColour();
        return ss.str();
    }




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
                           const char* fmt) {
        throw(Exception_(ptk::logging::LogRecord::formatException(exceptionType, file, line, fmt).toString()));
    }



} //namesapce logging
} //namespace ptk