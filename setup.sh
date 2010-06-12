#!/bin/bash

rm cpulse.so
rm `find -name cpulse.so`
sudo apt-get install libpulse-dev python-dev

set -e

python build/setup.py build
cp `find -name cpulse.so` .
