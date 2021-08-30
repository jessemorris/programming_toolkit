import re
import os
from ptk.utils import get_environment_vars
# from ptk.time import 


_DEFAULT_PTK_DIR = get_environment_vars("HOME") + "/.ptk/"
_DEFAULT_LOGGING_DIR = _DEFAULT_PTK_DIR + "logging/"
_DEFAULT_LOG_LEVEL = "INFO"

def _construct_ptk_root_dir():
    #all these should log to like a "ptkpy file"
    if os.path.isdir(_DEFAULT_PTK_DIR): #assumes logging is made with it
        return True

    #path does not exist so create it
    os.mkdir(_DEFAULT_PTK_DIR)
    os.mkdir(_DEFAULT_LOGGING_DIR)

def _reformat_directory(path):
    """[Reformates a directory path in the case where multiple "/"'s were appended to the path]

    Args:
        path ([str]): [Path]

    Returns:
        [str]: [Reformated path]
    """
    return re.sub('/+','/', path)

def _format_file_name(file_name):
    """[Reformats a string into a usable file name. Should be the NAME and not the file path
    as / will be removed, hence changing the file directory path.
    
    Replaces spaces and / with _]

    Args:
        file_name ([str]): [A string to turn into a file name]

    Returns:
        [str]: [Usable file name]
    """
    file_name = file_name.replace(" ", "_")
    file_name = file_name.replace("/", "_")
    return file_name


# def _construct_output_file(module_name, time, extension = "log"):
