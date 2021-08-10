#pragma once

#include "ptk/utils/macros.h"
#include <iostream>

namespace ptk {

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


    class Program {

        public:
            
            static bool init(int argc, char** argv);

            static const ProgramOptions* getProgramOptions();

        private:
            static bool is_init;
            static ProgramOptions program_options;




    };
    
} // namespace ptk
