#!/bin/bash

# Build the simulation
# Rely on CMake to find Geant4 from the environment (physino/geant4 base image)
cd /app/microelectronics_sim
mkdir -p build
cd build
cmake ..
make -j$(nproc)

# Run the simulation (1000 events)
./microelectronics_sim ../run.mac

# Run the analysis
python3 ../analyze.py
