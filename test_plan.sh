#!/bin/bash
echo "Testing replacement of g4root.hh and G4EmMicroElecPhysics.hh"
sed -i 's/#include "g4root.hh"/#include "G4AnalysisManager.hh"/' include/RunAction.hh

sed -i 's/#include "G4EmMicroElecPhysics.hh"/#include "MicroElecPhysics.hh"/' src/PhysicsList.cc
sed -i 's/G4EmMicroElecPhysics/MicroElecPhysics/' src/PhysicsList.cc

mkdir -p build && cd build && cmake .. && make -j$(nproc)
