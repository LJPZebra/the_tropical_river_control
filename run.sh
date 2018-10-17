#!/bin/bash

set -e
qmake GUI/TheTropicalRiver.pro
make
cd build
./TheTropicalRiver
cd ..


