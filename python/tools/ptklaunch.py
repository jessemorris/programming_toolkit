import ptk.core as ptk_core
import ptk.common as ptk_common
from ptk.core.process import ProcessLaunchConfig, ProcessManager
from ptk.utils import get_working_bash_directory
import argparse
import os
import ptk
import time

import sys

def _args_valid(args):
    if args.program_dir and args.config:
        print("Error launching program: Cannot load from config file and program args. See --help for usage.")
        sys.exit(0)

    if not args.program_dir and not args.config:
        print("Error launching program: Must specify --config or --program_dir. See --help for usage.")
        sys.exit(0)

def _construct_process_config(args):
    if args.config:
       return  _construct_config_from_file(args.config)
    elif args.program_dir:
        pass
    else:
        pass #shoudl never get here as _args_valid should check this

def _construct_config_from_file(config_paths):
    """[Constructs a set of ProcessLaunchConfig from the config path(s) given.
    Each config will represent a program to launch ]

    Args:
        config_paths ([List[str]]): [List of paths to configuration files to start a program with]
    Returns:
        launch_configs ([List[ProcessLaunchConfig]])
    """
    #list of ProcessLaunchConfig
    launch_configs = []
    pwd = get_working_bash_directory()
    for config_path in config_paths:
        #do we alwaays need to do this?
        config_path = ptk_core.file_system._reformat_directory(pwd + "/" + config_path)
        config = ptk_common.ConfigLoader(config_path)

        
        program_dir = config.getValue("program_dir")
        if program_dir is None:
            print("Error launching program: Must specify \"program_dir\" in the configuration file.")
            sys.exit(0)

        logging_dir = config.getValue("logging_dir")
        args = config.getValue("args")
        verbosity = config.getValue("verbosity")

        launch_configs.append(ProcessLaunchConfig(program_dir, config_path, logging_dir, args, verbosity))
    return launch_configs

    

def _construct_config_from_args(args):
    pass





def main():
    parser = argparse.ArgumentParser(description="Launcher to run programs. Using --config "
        "Must have a .yaml file in the form\n"
        "program_dir: <path to executable>\n"
        "logging_dir: <path to directory> [OPTIONAL]\n"
        "verbose: <verbosity level> [OPTIONAL, defaults to INFO]\n"
        "args: <list of any additional arguments> [OPTIONAL]\n")
    parser.add_argument("-c", "--config", type=str, nargs='+', help="Program directory. Required.")
    parser.add_argument("-p", "--program_dir", type=str, help="Program directory. Required.")
    parser.add_argument("-l", "--logging_dir", type=str, help="Logging directory. Required")
    parser.add_argument("-v", "--verbose", type=str, help="Level at which to log (DEBUG, INFO...). Optional.")
    parser.add_argument('argv', nargs='*')
    args = parser.parse_args()

    _args_valid(args)
    process_configs = _construct_process_config(args)

    manager = ProcessManager(process_configs)
    manager.startProcesses()

    while not ptk.is_shutdown() and manager.monitorProcesses():
        time.sleep(1)
        pass


if __name__ == "__main__":
    sys.exit(main())