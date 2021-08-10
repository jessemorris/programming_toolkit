import subprocess
import sys
import signal
import os
import time
import psutil
from threading import Thread

import ptk


if sys.version_info[0] < 3.5:
    signals_to_names_dict = dict((getattr(signal, n), n) \
        for n in dir(signal) if n.startswith('SIG') and '_' not in n )


def _signal_name_from_exitcode(code):
    if code is None:
        return None

    #code comes in an -N where N is POSIX exit code

    if sys.version_info[0] >= 3.5:
        return signal.Signals(abs(code)).name
    else:
        return signals_to_names_dict.get(abs(code), "Unnamed signal: %d" % abs(code))

    
def _construct_ptk_prgam_args(path, argv):
    """[Constrct ptk arguments as expected by program options. 
    This includes arguments in the form 
    -d, --pid=PID              Pid of the run program. Optional.
    -l, --logging_dir=DIR      Logging directory. Required.
    -n, --program_name=FILE    Name of the run program. Optional.
    -p, --program_dir=DIR      Program directory. Required.
    -v, --verbose=LOG          Level at which to log (DEBUG, INFO...). Optional.
    ]

    Args:
        path ([path to file]): [Full path to executable]
        argv ([List]): [List of additional args]

    Returns:
        [List]: [List of params in the correct form]
    """
    
    return []

def _construct_args(path, argv):
    program_type = path.split(".")[-1]

    cmd = []
    #TODO: this logic flow is weird -> pyton file extension vs exec?
    if program_type == "py":
        cmd = ['python3', path]
    elif os.access(path, os.X_OK):
        cmd = ['./'+path]
    else:
        cmd = [path]
        # print("Invalid file extension.")
        # sys.exit(0)

    cmd.extend(_construct_ptk_prgam_args(path, argv))

    return cmd

def _construct_program_name(path):
    return path.split("/")[-1]


class Statistics(object):

    def __init__(self):
        self._msg_count = 0
        self._data_array = []

    def addData(self, data):
        self._data_array.append(data)

    def getAvg(self):
        return sum(self._data_array)/len(self._data_array)

    def getMax(self):
        if len(self._data_array) > 0:
            return max(self._data_array)
        return -1

    def getMin(self):
        if len(self._data_array) > 0:
            return min(self._data_array)
        return -1
    



class Process(object):

    def __init__(self, path, argv):
        self.path = path
        self.argv = argv

        self.cmd = _construct_args(self.path, self.argv)
        self.program_name = _construct_program_name(self.path)
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
        self.popen = subprocess.Popen(self.cmd)
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
            print("Process alrady shutdown")
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
                print("Program {} [{}] ended with signal {}".format(self.program_name, self.pid, signal_result))
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




    