#!/bin/bash

# get required libs
sudo apt-get install libpulse-dev python-dev

set -e

# clean build directory
if [ -d "build" ]; then
    rm build -rf
fi

# compile and install cpulse
python setup/setup.py build
sudo python setup/setup.py install
