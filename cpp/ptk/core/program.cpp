#include "program.h"
#include "ptk/utils/logging.h"


#include <argp.h>
#include <stdbool.h>
#include <stdlib.h> 
#include <string.h>


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
                    arguments->program_dir = arg;
                    break;
                case 'l':
                    arguments->logging_dir = arg;
                    break;
                case 'n':
                    arguments->program_name = arg;
                    break;
                case 'v':
                    arguments->log_level = arg;
                    break;
                case 'd':
                    //should check for if digit
                    arguments->pid = (pid_t)atoi(arg);
                
                case ARGP_KEY_ARG: 
                    arguments->program_argc++;
                    if (arguments->program_argc == 1) {
                        arguments->program_argv = (char**) malloc(sizeof(char*));
                    }
                    else {
                        //make new memory
                        arguments->program_argv = (char**) realloc(arguments->program_argv, 
                            arguments->program_argc * sizeof(char*));
                    }   
                    arguments->program_argv[arguments->program_argc-1] = arg;
                    break;

                default: return ARGP_ERR_UNKNOWN;
            }   
            return 0;
        }

        static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };

    }

    bool Program::is_init = false;
    ProgramOptions Program::program_options({});

    bool Program::init(int argc, char** argv) {
        if (is_init) {
            PTK_WARN_MSG("Program has already been initalised.");
            return false;
        }
        program_options.program_argc = 0;
        program_options.logging_dir = 0;
        program_options.program_dir = 0;
        const char* default_log = "INFO";
        program_options.log_level = (char*)default_log; //default


       
        argp_parse(&impl::argp, argc, argv, 0, 0, &program_options);
        
        if (!program_options.logging_dir || !program_options.program_dir) {
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
            return nullptr;
        }
    }


}