from inspect import getattr_static
import os
import sys
from threading import Lock
import psutil


def get_environment_vars(key:str):
    """[Gets a value from the environment varibles. System exists if the key cannot be found]

    Args:
        key ([str]): [key]

    Returns:
        [str]: [Environ variable.]
    """
    try:
        return os.environ[key]
    except KeyError:
        print("{} not set. System exiting.".format(key))
        sys.exit(0)

    
def merge_dictionaries(a, b):
    """[Helper function to merge two dictioanries together. ]

    Args:
        a ([dict]): [description]
        b ([dict]): [description]

    Returns:
        [dict]: [description]
    """
    z = a.copy()
    z.update(b)
    return z


def isclassmethod(method):
    """[Heper function to determine of method is a class method.
    
    Eg.
    class Test():

        ...
        @classmethod
        def from_thing(cls, data):
            return cls(data)

    > isclassmethod(Test.from_thing)
    True
    
    ]

    Args:
        method ([callable]): [The function to test]

    Returns:
        [bool]: [True if method is a class method]
    """
    bound_to = getattr(method, '__self__', None)
    if not isinstance(bound_to, type):
        return False
    name = method.__name__
    for cls in bound_to.__mro__:
        descriptor = vars(cls).get(name)
        if descriptor is not None:
            return isinstance(descriptor, classmethod)
    return False

def isstaticmethod(classtype, method_name:str):
    """[Helper function to determine if a method is a static method.]

    Eg.

    Class Test():
        ...

        @staticmethod
        def func():
            ...

    > isstaticmethod(Test, 'func')
    True

    Args:
        classtype ([type]): [The type of object.]
        method_name (str): [The name of the function to test.]

    Returns:
        [bool]: [True if the method specified is static]
    """
    return isinstance(getattr_static(classtype, method_name), staticmethod)


import inspect
def inspect_frame(index = 1):
    """[Returns the frame information about a specific stack at some index.
    
    Allows internal reflection on function calls to determine from which file, line number and 
    function's were previously called.]

    Args:
        index (int, optional): [How many stacks to pop before returning the desired information.]. Defaults to 1.

    Returns:
        [str, str, str]: [The filename, line number and funtion name in the stack speicifed by the index]
    """
    frame,filename,line_number,function_name,lines,index = inspect.stack()[index]
    return filename, line_number, function_name

def is_process(process_name):
    """[Check if the running process is the same as the process name ie. we are that process]

    Args:
        process_name ([str]): [The name of the process to check. In the case of a C style executable (or an installed python 
        script) this will just be the name of the install program.]

    Returns:
        [bool]: [True if the process exists]
    """
    pid = os.getpid()
    #Iterate over the all the running process
    for proc in psutil.process_iter():
        try:
            # Check if process name contains the given name string.
            if process_name.lower() in proc.name().lower() and proc.pid == pid:
                return True
        except (psutil.NoSuchProcess, psutil.AccessDenied, psutil.ZombieProcess):
            pass
    return False
