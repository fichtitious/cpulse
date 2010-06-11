#!/bin/bash

rm cpulse.so
rm `find -name cpulse.so`
sudo apt-get install libpulse-dev aubio-tools libaubio-dev python-dev

set -e

python build/setup.py build
cp `find -name cpulse.so` .
