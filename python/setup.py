import os
from setuptools import setup, find_packages
import glob
import shutil
import pwd

here = os.path.dirname(os.path.abspath(__file__))


def read(fname):
    return open(os.path.join(os.path.dirname(__file__), fname)).read()

print(find_packages())

setup(
    name = "ptk",
    version = "0.0.1",
    author = "Jesse Morris",
    author_email = "jessmmorris7@gmail.com",
    description = ("Programming Toolkit for Python"),
    license = "BSD",
    keywords = "example documentation tutorial",
    packages=find_packages(),
    long_description=read('README.md'),
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Topic :: Utilities",
        "License :: OSI Approved :: BSD License",
    ],
    entry_points={
        'console_scripts': [
            'ptkcv_thresholding=tools.ptk_cv_thresholding:main',
            'ptklaunch=tools.ptklaunch:main'
        ]
    }
)
