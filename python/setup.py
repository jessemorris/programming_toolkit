import os
from setuptools import setup, find_packages
import glob
import shutil
import pwd


def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

    


setup(
    name = "ptk",
    version = "0.0.1",
    author = "Jesse Morris",
    author_email = "jessmmorris7@gmail.com",
    description = ("Programming Toolkit for Python"),
    license = "BSD",
    keywords = "example documentation tutorial",
    packages=['ptk'],
    long_description=read('README.md'),
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Topic :: Utilities",
        "License :: OSI Approved :: BSD License",
    ],
    # entry_points={
    #     'console_scripts': [
    #         'bioscout_server=bioscout.executables.main_core:main',
    #         'bioscout_param=bioscout.executables.main_param_query:main',
    #         'bioscout_srv=bioscout.executables.main_srv_query:main',
    #     ]
    # }
)
