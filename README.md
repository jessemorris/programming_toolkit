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