#pragma once

#include "ptk/utilities/macros.h"
#include <iostream>
#include <string.h>
#include <string>
#include <vector>

namespace ptk {

    //this might need to be user home + 

    /**
     * @brief Defines a set of options for this program and are available from the Program class.
     * This options define system level information that is configurable using a yaml file. Using 
     * 'ptklaunch' and then using Program::init will automatically fill out these members.
     * 
     */
    struct ProgramOptions {
        

        PTK_POINTER_TYPEDEFS(ProgramOptions);
        using ProgramArgvs = std::vector<std::string>;

        std::string program_dir;
        std::string logging_dir; //TODO: default options
        std::string program_name;
        std::string log_level;
        ProgramArgvs program_argv;
        int program_argc;
        pid_t pid;

        friend std::ostream& operator<<(std::ostream& out, const ProgramOptions& object) {
			out << "Logging dir: " << object.logging_dir << std::endl;
			return out;
		}

    };

    /**
     * @brief Wrapper class to define system level confguration via the command line arguments when
     * using the ptklaunch tool. 
     * 
     */
    class Program {

        public:
            /**
             * @brief Inalises and populates static ProgramOptions struct when the program is called with the
             * valid arguments. This can be done manually or using the ptklaunch tool and a corresponding launch file.
             * 
             * @param argc 
             * @param argv 
             * @return true 
             * @return false 
             */
            static bool init(int argc, char** argv);

            /**
             * @brief Prints the program options to the terminal window. 
             * 
             */
            static void displayProgramOptions();

            /**
             * @brief Get the Program Options object
             * 
             * @return const ProgramOptions* 
             */
            static const ProgramOptions* getProgramOptions();

        private:
            static bool is_init;
            static ProgramOptions program_options;




    };
    
} // namespace ptk
