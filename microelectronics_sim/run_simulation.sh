#!/bin/bash

# Build the simulation
cd /app/microelectronics_sim
mkdir -p build
cd build
cmake -DGeant4_DIR=$GEANT4_DIR/lib/cmake/Geant4 ..
make -j$(nproc)

# Run the simulation (1000 events)
./microelectronics_sim ../run.mac

# Run the analysis
python3 ../analyze.py
