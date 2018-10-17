#!/bin/bash

set -e
qmake GUI/Fishy.pro
make
cd build
./Fishy
cd ..


