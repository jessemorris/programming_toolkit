import subprocess
import sys
import signal
import os
import time
import psutil
from threading import Thread

import ptk
from ptk.logging import _logging_to_level_names, logerror, loginfo
from ptk.core import _DEFAULT_LOG_LEVEL, _DEFAULT_LOGGING_DIR, _reformat_directory, file_system
from ptk.common.types import Statistics
from ptk.common.exceptions import *



if sys.version_info[0] < 3.5:
    signals_to_names_dict = dict((getattr(signal, n), n) \
        for n in dir(signal) if n.startswith('SIG') and '_' not in n )


def _signal_name_from_exitcode(code):
    if code is None:
        return None

    if code is 0:
        return 0 #finished successfully

    #code comes in an -N where N is POSIX exit code

    if sys.version_info[0] >= 3.5:
        return signal.Signals(abs(code)).name
    else:
        return signals_to_names_dict.get(abs(code), "Unnamed signal: %d" % abs(code))



#could put in core?
def _construct_file_name(path:str):
    """[Gets the file (or executable) from a file path. Typically will be
    the last part of the folder path eg "path/to/program", the program will be called "program".]

    Args:
        path ([str]): [Absoltue File path]

    Returns:
        [str]: [file name]
    """
    return path.split("/")[-1]


def _construct_dir_from_path(path:str):
    """[From a file path, returns the folder that the file is in. Eg from a file path
    path/to/program.c, the return value will be "path/to/"]

    Args:
        path (str): [description]

    Returns:
        [type]: [description]
    """
    split_path = path.split("/")[:-1]
    dir_str = ""

    for f in split_path:
        dir_str += ("/" + f)

    return _reformat_directory(dir_str)




class ProcessLaunchConfig(object):
    """[Specifies the program options to launch process with. 
    Internally constructs the commands required to create the subprocess using Popen.]

    """

    def __init__(self, program_dir, config_file,logging_dir=None, args = None, verbosity="INFO"):
        self._config_file = config_file
        self._program_dir = self._construct_program_dir(program_dir, config_file)

        self._program_name = _construct_file_name(self._program_dir) #must happen before constructing logging dir
        self._args = args
        self._verbosity = self._construct_logging_level(verbosity)
        self._logging_dir = self._construct_logging_dir(logging_dir)

    def __repr__(self) -> str:
        repr = "Process Configuration:\n"
        repr += " - program name: {}\n".format(self._program_name)
        repr += " - program dir: {}\n".format(self._program_dir)
        repr += " - logging dir: {}\n".format(self._logging_dir)
        repr += " - verbosity: {}\n".format(self._verbosity)
        repr += " - args: {}\n".format(self._args)
        return repr

    def generate_popen_command(self):
        """[Constrct ptk arguments as expected by program options. 
            Includes infering the correct execution command for the given script
            (python, executables, or binaries). There is no plan to support other languages.
            This includes arguments in the form 
            -d, --pid=PID              Pid of the run program. Optional.
            -l, --logging_dir=DIR      Logging directory. Required.
            -n, --program_name=FILE    Name of the run program. Optional.
            -p, --program_dir=DIR      Program directory. Required.
            -v, --verbose=LOG          Level at which to log (DEBUG, INFO...). Optional.]

        Returns:
            [cmd]: [The list of commands, prefixed by the execution command. ]
        """
        program_type = self._program_dir.split(".")[-1]

        cmd = []
        #TODO: this logic flow is weird -> pyton file extension vs exec?
        if program_type == "py":
            cmd = ['python3', self._program_dir]
        else:
            cmd = [self._program_dir]

        cmd.extend(["--program_dir", self._program_dir])
        cmd.extend(["--logging_dir", self._logging_dir])
        cmd.extend(["--program_name", self._program_name])
        cmd.extend(["--verbose", self._verbosity])

        return cmd

    
    def _construct_program_dir(self, program_dir, config_file):
        """[Constructs the full path to the program from the given program dir and the config file.
        The path is checked as a absolute file path and then as a path relatie to the same folder that the
        config file resides in. After checking both locations (if the file exists), it is checked
        to be an executable program. If true, the path where the file resides will be returned.]

        Args:
            program_dir ([str]): [Absolute or relaitve (to the config file) path to the executable]
            config_file ([str]): [Absoltue path to the executable.]

        Returns:
            [str]: [Absolute program path]
        """
        p_dir = program_dir
        if not os.path.isfile(p_dir):
            #file doesnt exist so try relative to config file
            config_dir = _construct_dir_from_path(config_file)
            p_dir = _reformat_directory(config_dir + "/" + program_dir)

            if not os.path.isfile(p_dir):
                print("Program executable could not be found by its full path or relative to the configuration."
                "Checked paths:\n {}\n{}".format(program_dir, p_dir))
                sys.exit(0)

        #check for permission
        if not os.access(p_dir, os.X_OK):
            print("Program path [{}] does not specify a path to an executable. Try checking "
            "the permissions...".format(p_dir))
            sys.exit(0)

        return p_dir


    def _construct_file_name(self):
        """[Gets the file (or executable) fromt the program directory. Typically will be
        the last part of the folder path eg "path/to/program", the program will be called "program".]

        Returns:
            [str]: [Program name]
        """
        return self._program_dir.split("/")[-1]


    def _construct_logging_dir(self, logging_dir):
        """[Constructs the logging directory to use. If None, the default directory will be used 
        with the program name as extension eg "./ptk/logging/" + "program_name".]

        Args:
            logging_dir ([str]): [The directory or None]

        Returns:
            [str]: [The logging dir]
        """
        if logging_dir is None:
            return _reformat_directory(_DEFAULT_LOGGING_DIR + "/" + self._program_name)
        else:
            return logging_dir

    def _construct_logging_level(self, verbosity):
        """[Constructs a valid verbosity string to use (DEBUG, INFO etc). 
        If the input is invalid the default logging level will be used ]

        Args:
            verbosity ([str]): [level to log at]

        Returns:
            [str]: [Valid verbosity]
        """
        if verbosity is None:
            return _DEFAULT_LOG_LEVEL
        if verbosity not in _logging_to_level_names:
            print("Invalid verbosity [{}]. Must be one of DEBUG, INFO etc... Setting to {}.".format(verbosity, _DEFAULT_LOG_LEVEL))
            return _DEFAULT_LOG_LEVEL
        return verbosity


class Process(object):

    def __init__(self, process_config: ProcessLaunchConfig):
        self.process_config = process_config

        self.cmd = self.process_config.generate_popen_command()
        self.program_name = self.process_config._program_name
        self.reporting_thread = Thread(target=self._reporting_worker)
        self.popen = None
        self.pid = -1
        self.process_result = 0
        self.process_monitoring = None
        self.process_stats_cpu = Statistics()
        self.process_stats_mem = Statistics()
        self.is_shutdown = False
        self.process_active = False
        self.program_start_time = None 
        self.program_end_time = None

    def start(self):
        if self.popen is not None and self.isAlive():
            print("Process [{}] already started".format(self.pid))
            return True

        try:
            self.popen = subprocess.Popen(self.cmd)
        except Exception as e:
            raise LaunchConfigException("Popen failed: {}".format(str(e)))

        self.process_result = self.popen.poll()

        if self.process_result  is None or self.process_result  == 0:
            self.pid = self.popen.pid
            print("Started process {}".format(self.pid))
            self.program_start_time = time.time()
            self.process_active = True
            self.process_monitoring = psutil.Process(self.pid)
            self.reporting_thread.start()
            return True 
        else:
            print("Process failed to start")
            return False

    def isAlive(self):
        return ptk.utils.is_process_alive(self.pid)

    def shutdown(self):
        self.is_shutdown = True

    def _shutdown_subprocess(self):

        #this is just a flag -> could actively check if pid active?
        if not self.isAlive():
            # print("Process [{}] already shutdown".format(self.pid))
            return True


        self.process_active = False
        try:
            print("Shutting down process {}".format(self.pid))
            self.popen.kill()
            return True
        except Exception as e:
            return False


    def _reporting_worker(self):
        while not self.is_shutdown:
            poll_result = self.popen.poll()
            signal_result = _signal_name_from_exitcode(poll_result)

            if signal_result is not None:
                if signal_result is not 0:
                    logerror("Program {} [{}] ended with signal {}".format(self.program_name, self.pid, signal_result))
                else:
                    loginfo("Program {} [{}] ended with signal code 0".format(self.program_name, self.pid))
                break

            cpu_percent = self.process_monitoring.cpu_percent()
            self.process_stats_cpu.addData(cpu_percent)

            mem_percent = self.process_monitoring.memory_percent()
            self.process_stats_mem.addData(mem_percent)

            self.program_end_time = time.time()
            time.sleep(1) #give CPU some sleep time so while loop isnt running at super high Hz

        self._shutdown_subprocess()

    def getProcessStats(self):
        stats = ("Program: {} [{}]\n - runnimg time {}"
            "\n - memory usage"
            "\n\t* avg {:.5f} [max {:.5f} min {:.5f}]"
            "\n - CPU usage"
            "\n\t* avg {:.5f} [max {:.5f} min {:.5f}]"
        ).format(self.program_name, self.pid, (self.program_end_time - self.program_start_time),
            self.process_stats_mem.getAvg(),
            self.process_stats_mem.getMax(),
            self.process_stats_mem.getMin(),
            self.process_stats_cpu.getAvg(),
            self.process_stats_cpu.getMax(),
            self.process_stats_cpu.getMin())
        return stats



class ProcessManager(object):

    def __init__(self, process_configs):
        """[Manages a set of Programs that can be launched and is responsible for starting and shutting them down.
         Upon the ptk sigint handler, the processes will be shutdown.]

        Args:
            process_configs ([List[ProcessLaunchConfig]]): [List of validaded ProcessLaunchConfig's]
        """
        self._process_list = []
        for process in process_configs:
            # print(process)
            self._process_list.append(Process(process))

        ptk.registerExitCallback(self.shutdownProcesses)

    def startProcesses(self):
        for p in self._process_list:
            p.start()

    def shutdownProcesses(self):
        for p in self._process_list:
            print(p.getProcessStats())
            p.shutdown()

    def monitorProcesses(self):
        """[Checks if any process are running. Returns true of there is at least one process still running
        and False if they are all shutdown]

        Returns:
            [bool]: [Result]
        """
        result = []
        for p in self._process_list:
            result.append(p.isAlive())

        if sum(result) == 0: #ie. they are all False
            return False #all proceses are shut down
        return True # there is at least one process running
    