#!/bin/bash

rm _cpulse.so
rm `find -name _cpulse.so`

set -e

sudo apt-get install libpulse-dev aubio-tools libaubio-dev python-dev
python build/setup.py build
cp `find -name _cpulse.so` .
