#!/bin/bash

rm cpulse.so
rm `find -name cpulse.so`

set -e

sudo apt-get install libpulse-dev aubio-tools libaubio-dev python-dev
python build/setup.py build
cp `find -name cpulse.so` .
