#!/bin/bash

if [ -d "build" ]; then
    sudo rm build -rf
fi

sudo apt-get install libpulse-dev python-dev

set -e

sudo python setup/setup.py build install
