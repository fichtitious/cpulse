#!/bin/bash

if [ -d "build" ]; then
    rm build -rf
fi

sudo apt-get install libpulse-dev python-dev

set -e

python setup/setup.py build
sudo python setup/setup.py install
