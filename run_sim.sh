#!/bin/bash
cd /app/microelectronics_sim
mkdir -p build
cd build
cmake ..
make -j4
./microelectronics_sim ../run.mac
