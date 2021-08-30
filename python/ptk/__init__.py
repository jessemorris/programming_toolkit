from .logging import *
from .common import *
from .core import *



import signal
import time


from .core.file_system import _construct_ptk_root_dir


global _is_shutdown
_is_shutdown = False
_sig_handler_init = False
_exit_callbacks = []
_origianl_sig_handler = None

def is_shutdown():
    return _is_shutdown

def registerExitCallback(func):
    if not callable(func):
        raise TypeError("shutdown hook [%s] must be a function or callable object: %s"%(func, type(func)))
    _exit_callbacks.append(func)

def _handler(signum, frame):
    signal.signal(signal.SIGINT, _origianl_sig_handler)
    global _is_shutdown
    _is_shutdown = True
    global _exit_callbacks
    for cb in _exit_callbacks:
        cb()
    print("PTK: SIGINT recieved. Shutting down")
    # import sys
    # time.sleep(3) #wait some time to ensure threads are shutdown 
    # sys.exit(0)

def _init_ptk_signal_handler():
    global _sig_handler_init
    if _sig_handler_init is False:
        global _origianl_sig_handler
        _origianl_sig_handler = signal.getsignal(signal.SIGINT)
        signal.signal(signal.SIGINT, _handler)
        _sig_handler_init = True

_init_ptk_signal_handler()
_construct_ptk_root_dir()