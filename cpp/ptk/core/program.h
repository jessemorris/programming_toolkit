#pragma once

#include "ptk/utils/macros.h"
#include <iostream>

namespace ptk {

    /**
     * @brief Defines a set of options for this program and are available from the Program class.
     * This options define system level information that is configurable using a yaml file. Using 
     * 'ptklaunch' and then using Program::init will automatically fill out these members.
     * 
     */
    struct ProgramOptions {
        

        PTK_POINTER_TYPEDEFS(ProgramOptions);

        char* program_dir;
        char* logging_dir;
        char* program_name;
        char* log_level;
        char** program_argv;
        int program_argc;
        pid_t pid;

        friend std::ostream& operator<<(std::ostream& out, const ProgramOptions& object) {
			out << "Progam dir: " << object.program_dir << std::endl;
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
