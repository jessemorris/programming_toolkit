# Programming Toolkit

Set of tools, classes and functions that I find myself using often or what to use as a guide when structuring other projects. 

Currently building for cpp and python projects.

## Building

### CPP
```
mkdir build && cd build
cmake ..
make
sudo make install
```

To build with example use ```-DBUILD_EXAMPLES=ON```
To build with unit tests use ```-DBUILD_UNIT_TESTS=ON```


### Python
```
sudo python3 setup.py install
```

### Bash Script
```
sudo ./installPTK.sh
```
Works but not fully tested. May need to ensure the system is cleaned before this is run.

### Cleaning
TODO

## Using PTK in other projects

## Infrastructure 
ptk_pp and ptk_python both build as relatively standard librariers as per their language. 
### CPP
The CPP library builds and installs its libraries to ```/usr/loca/lib``` and cmake config to ```/usr/local/lib/cmake ```. Plugins installed with __install_plugins__ go to ```/usr/loca/lib/ptk_plugins```. Header files are installed to ```/usr/local/include/ptk/``` with each module getting its own folder space.

### Python
The python library is installed to ``` /usr/local/lib/python3.6/dist-packages``` as an egg module. Some issues with getting this to be seen by text editors but python can find it fine. Only support for Python3.6 currently.

Additional command line tools are installed to ```/usr/local/bin``` so as to be directly on the file path.

### Packages
Each package should be build uder its own namespace corresponding to the package it is in, ie. __ptk::package__ 
#### Utilities 


#### Core 

#### Plugins

#### Common

TODO: not sure the difference between common and core at this stage. Common should be key classes that we use often but do not have a specific application, 
I think core should be things used internally (for the most part) and utils will general utils (file system, logging) that is used internall externall
## Namespacing
Everything (other than macros) MUST be put under the __ptk__ namespace.
- Work in progress

Current namespaces:
- plugins
- utils (make utilities?)
- impl
  - This namepspace should be used to indicate that the class/functions are for internal use only and should not be used by the users of the library. 