import pickle
import traceback
import inspect
import logging
import os
import sys
from enum import Enum

from .common.time import Time, Duration




class PtkLogger(logging.getLoggerClass()):
    def findCaller(self, *args, **kwargs):
        """
        Find the stack frame of the caller so that we can note the source
        file name, line number, and function name with class name if possible.
        """
        file_name, lineno, func_name = super(PtkLogger, self).findCaller(*args, **kwargs)[:3]
        file_name = os.path.normcase(file_name)

        f = inspect.currentframe()
        if f is not None:
            f = f.f_back
        while hasattr(f, "f_code"):
            # Search for the right frame using the data already found by parent class.
            co = f.f_code
            filename = os.path.normcase(co.co_filename)
            if filename == file_name and f.f_lineno == lineno and co.co_name == func_name:
                break
            if f.f_back:
                f = f.f_back

        # Jump up two more frames, as the logger methods have been double wrapped.
        if f is not None and f.f_back and f.f_code and f.f_code.co_name == '_base_logger':
            f = f.f_back
            if f.f_back:
                f = f.f_back
        co = f.f_code
        func_name = co.co_name

        # Now extend the function name with class name, if available.
        try:
            class_name = f.f_locals['self'].__class__.__name__
            func_name = '%s.%s' % (class_name, func_name)
        except KeyError:  # if the function is unbound, there is no self.
            pass

        if sys.version_info > (3, 2):
            # Dummy last argument to match Python3 return type
            return co.co_filename, f.f_lineno, func_name, None
        else:
            return co.co_filename, f.f_lineno, func_name

logging.setLoggerClass(PtkLogger)


def _frame_to_caller_id(frame):
    caller_id = (
        inspect.getabsfile(frame),
        frame.f_lineno,
        frame.f_lasti,
    )
    return pickle.dumps(caller_id)


import os
from datetime import date



def _base_logger(msg, args, kwargs, throttle=None,
                 throttle_identical=False, level=None, once=False):

    ptk_logger = logging.getLogger('ptkout')

    if not ptk_logger.hasHandlers():
        stream_hander = PtkStreamHandler()
        ptk_logger.addHandler(stream_hander)

    name = kwargs.pop('logger_name', None)
    if name:
        ptk_logger = ptk_logger.getChild(name)
    logfunc = getattr(ptk_logger, level)

    ptk_logger.setLevel(logging.INFO)


    if once:
        caller_id = _frame_to_caller_id(inspect.currentframe().f_back.f_back)
        if _logging_once(caller_id):
            logfunc(msg, *args, **kwargs)
    elif throttle_identical:
        caller_id = _frame_to_caller_id(inspect.currentframe().f_back.f_back)
        throttle_elapsed = False
        if throttle is not None:
            throttle_elapsed = _logging_throttle(caller_id, throttle)
        if _logging_identical(caller_id, msg) or throttle_elapsed:
            logfunc(msg, *args, **kwargs)
    elif throttle:
        caller_id = _frame_to_caller_id(inspect.currentframe().f_back.f_back)
        if _logging_throttle(caller_id, throttle):
            logfunc(msg, *args, **kwargs)
    else:
        logfunc(msg, *args, **kwargs)

#These will act as normal log function
def logdebug(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, level='debug')

def loginfo(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, level='info')

def logwarn(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, level='warn')

def logerror(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, level='error')

def logfatal(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, level='critical')

class LoggingThrottle(object):

    last_logging_time_table = {}

    def __call__(self, caller_id, period):
        """Do logging specified message periodically.
        - caller_id (str): Id to identify the caller
        - logging_func (function): Function to do logging.
        - period (float): Period to do logging in second unit.
        - msg (object): Message to do logging.
        """
        now = Time.now()

        last_logging_time = self.last_logging_time_table.get(caller_id)

        if (last_logging_time is None or
              (now - last_logging_time) > Duration(period)):
            self.last_logging_time_table[caller_id] = now
            return True
        elif last_logging_time > now:
            self.last_logging_time_table = {}
            self.last_logging_time_table[caller_id] = now
            return True
        return False


_logging_throttle = LoggingThrottle()

#These will log periodically (given a period in seconds) within a loop
def logdebug_throttle(period, msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle=period, level='debug')

def loginfo_throttle(period, msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle=period, level='info')

def logwarn_throttle(period, msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle=period, level='warn')

def logerror_throttle(period, msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle=period, level='error')

def logfatal_throttle(period, msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle=period, level='critical')


from hashlib import md5
class LoggingIdentical(object):

    last_logging_msg_table = {}

    def __call__(self, caller_id, msg):
        """Do logging specified message only if distinct from last message.
        - caller_id (str): Id to identify the caller
        - msg (str): Contents of message to log
        """
        msg_hash = md5(msg.encode()).hexdigest()

        if msg_hash != self.last_logging_msg_table.get(caller_id):
            self.last_logging_msg_table[caller_id] = msg_hash
            return True
        return False


_logging_identical = LoggingIdentical()


#These will log only on message changes
def logdebug_identical(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle_identical=True,
                level='debug')

def loginfo_identical(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle_identical=True,
                level='info')

def logwarn_identical(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle_identical=True,
                level='warn')

def logderror_identical(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle_identical=True,
                level='error')

def logfatal_identical(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle_identical=True,
                level='critical')



#These will log only idenfical messages period
def logdebug_throttle_identical(period, msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle=period, throttle_identical=True,
                 level='debug')

def loginfo_throttle_identical(period, msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle=period, throttle_identical=True,
                 level='info')

def logwarn_throttle_identical(period, msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle=period, throttle_identical=True,
                 level='warn')

def logerror_throttle_identical(period, msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle=period, throttle_identical=True,
                 level='error')

def logfatal_throttle_identical(period, msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, throttle=period, throttle_identical=True,
                 level='critical')


class LoggingOnce(object):

    called_caller_ids = set()

    def __call__(self, caller_id):
        if caller_id not in self.called_caller_ids:
            self.called_caller_ids.add(caller_id)
            return True
        return False

_logging_once = LoggingOnce()

#These will only log once regardless
def logdebug_once(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, once=True, level='debug')

def loginfo_once(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, once=True, level='info')

def logwarn_once(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, once=True, level='warn')

def logerror_once(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, once=True, level='error')

def logfatal_once(msg, *args, **kwargs):
    _base_logger(msg, args, kwargs, once=True, level='critical')



class Level(Enum):
    DEBUG = 0,
    INFO = 1,
    WARN = 2,
    ERROR = 3,
    EXCEPTION = 4

_logging_to_level_names = {
    'DEBUG': ('DEBUG', '\033[32m'),
    'INFO': ('INFO', None),
    'WARNING': ('WARN', '\033[33m'),
    'ERROR': ('ERROR', '\033[31m'),
    'CRITICAL': ('FATAL', '\033[31m')
}

_level_to_quantity = {
    Level.DEBUG: 0,
    Level.INFO: 1,
    Level.WARN: 2,
    Level.ERROR: 3,
    Level.EXCEPTION: 4
}



_color_reset = '\033[0m'

LOGGING_FILE_FORMAT = '[${time}] [${severity}] [${file}:${lineno}]: ${message}'
_defaultFormatter = logging.Formatter()

import os


_global_logging_level = Level.INFO

def setPtkLoggingLevel(level):
    if not isinstance(level, Level):
        logerror("Level must be an instance of ptk.logging.Level")
        return

    _global_logging_level = level

    




class PtkStreamHandler(logging.Handler):
    def __init__(self, colorize=True, stdout=None, stderr=None):
        super(PtkStreamHandler, self).__init__()
        self._colorize = colorize

    def emit(self, record):
        level, color = _logging_to_level_names[record.levelname]
        
        msg = LOGGING_FILE_FORMAT
        msg = msg.replace('${severity}', level)
        msg = msg.replace('${message}', str(record.msg))

        msg = msg.replace('${time}', '%s' % Time.now())  # for performance reasons
        msg = msg.replace('${file}', '%s' % record.filename) 
        msg = msg.replace('${lineno}', '%s' % record.lineno) 

        record_name =  str(record.name)
        # log_path = "~/Code/" + record_name + ".log"

        # try:
        #     from rospy import get_name
        #     node_name = get_name()
        # except ImportError:
        #     node_name = '<unknown_node_name>'
        # msg = msg.replace('${node}', node_name)

        msg += '\n'
        # print(msg)
        self._write(sys.stdout, msg, color)

    def _write(self, fd, msg, color):
        if self._colorize and color and hasattr(fd, 'isatty') and fd.isatty():
            msg = color + msg + _color_reset
        fd.write(msg)


def sort_logging_files(*args):
    pass


# add_exception_hook(sort_logging_files)