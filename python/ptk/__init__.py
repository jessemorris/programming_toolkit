from .logging import *
from .common import *



import signal
import time

global _is_shutdown
_is_shutdown = False
_exit_callbacks = []
_origianl_sig_handler = signal.getsignal(signal.SIGINT)

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
    import sys
    time.sleep(3) #wait some time to ensure threads are shutdown 
    sys.exit(0)

def _init_ptk_signal_handler():
    signal.signal(signal.SIGINT, _handler)

_init_ptk_signal_handler()