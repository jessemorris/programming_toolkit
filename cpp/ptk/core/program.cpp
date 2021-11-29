#include "program.h"
#include "ptk/utilities/logging.h"
#include "ptk/utilities/fileSystem.hpp"
#include "ptk/core/impl-logger.hpp"


#include <argp.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <string.h>


//TODO: really should put program until utils (rename utility)

namespace ptk {

    namespace impl {

        static char doc[] = "Your program description.";
        static char args_doc[] = "[FILENAME]...";

        static struct argp_option options[] = { 
            { "program_dir", 'p', "DIR", 0, "Program directory. Required."},
            { "logging_dir", 'l',  "DIR", 0, "Logging directory. Required."},
            { "program_name", 'n',"FILE", 0, "Name of the run program. Optional."},
            { "verbose", 'v', "LOG", 0, "Level at which to log (DEBUG, INFO...). Optional."},
            { "pid", 'd', "PID", 0, "Pid of the run program. Optional."}, //actually unused becuase we can get pid interally
            { 0 } 
        };

        static error_t parse_opt(int key, char *arg, struct argp_state *state) {
            //need static cast from C to C++ style struct!
            struct ProgramOptions *arguments = static_cast<struct ProgramOptions*>(state->input);
            switch (key) {
                case 'p':
                    arguments->program_dir = std::string(arg);
                    break;
                case 'l':
                    arguments->logging_dir =  std::string(arg);
                    break;
                case 'n':
                    arguments->program_name =  std::string(arg);
                    break;
                case 'v':
                    arguments->log_level =  std::string(arg);
                    break;
                case 'd':
                    //should check for if digit
                    arguments->pid = (pid_t)atoi(arg);
                
                case ARGP_KEY_ARG: 
                    arguments->program_argc++;
                    // if (arguments->program_argc == 1) {
                    //     arguments->program_argv = (char**) malloc(sizeof(char*));
                    // }
                    // else {
                    //     //make new memory
                    //     arguments->program_argv = (char**) realloc(arguments->program_argv, 
                    //         arguments->program_argc * sizeof(char*));
                    // }   
                    arguments->program_argv.push_back(std::string(arg));
                    // arguments->program_argv[arguments->program_argc-1] = arg;
                    break;

                default: return ARGP_ERR_UNKNOWN;
            }   
            return 0;
        }

        static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

    } //impl

    bool Program::is_init = false;
    ProgramOptions Program::program_options;

    bool Program::init(int argc, char** argv) {
        if (is_init) {
            PTK_WARN_MSG("Program has already been initalised.");
            return false;
        }
        // program_options.program_argc = ;
        program_options.logging_dir = ptk::getDefaultPtkLoggingDir();
        program_options.program_dir = "";
        program_options.log_level = "INFO"; //TODO: default

        utils::impl::getInternalLogger()->log("Test log interal");

       
        argp_parse(&impl::argp, argc, argv, 0, 0, &program_options);
        
        if (program_options.logging_dir.empty() || program_options.program_dir.empty()) {
            PTK_ERROR_MSG("Incomplete arguments. See usage --help.");
            return false;
        }

        is_init = true;



    }

    const ProgramOptions* Program::getProgramOptions() {
        if (is_init) {
            return &program_options;
        }
        else {
            std::cout << "PTK program arguments have not been initalised with init" << std::endl;
            //TODO: set default values for logging folder or exit if init not called
            return &program_options;
        }
    }

    void Program::displayProgramOptions() {
        // if(is_init) {
            std::cout << "Program Options:" << std::endl;
            std::cout << "\t* Program dir: " << std::string(program_options.program_dir) << std::endl;
            std::cout << "\t* Logging dir: " << std::string(program_options.logging_dir) << std::endl;
            std::cout << "\t* Log level: " << std::string(program_options.log_level) << std::endl;
            std::cout << "\t* Program name: " << std::string(program_options.program_name) << std::endl;
        // }
        // else {
        //     PTK_ERROR_MSG("Cannot print program options: null");
        // }
    }


}