#!/bin/bash

echo 'Starting Install PTK script...'

echo 'Install PTK.a lib..'
cd build && cmake .. && make  -j$(nproc) install
echo 'Done...'
cd ../python
python3 setup.py install