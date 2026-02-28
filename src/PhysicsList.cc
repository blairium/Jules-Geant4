#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "MicroElecPhysics.hh"
#include "G4EmStandardPhysics_option4.hh" // Standard EM physics for higher energy/other particles
#include "G4EmLowEPPhysics.hh" // Low energy package if needed, but option4 is generally robust

PhysicsList::PhysicsList()
: G4VModularPhysicsList()
{
  SetVerboseLevel(1);

  // Standard EM Physics (Option 4):
  // We include this to handle general electromagnetic interactions, especially for the primary photons
  // (Photoelectric effect, Compton scattering, etc.) and for particle transport outside of the
  // specific microelectronics regions or for particles not handled by MicroElec physics.
  // Option4 is generally the most accurate standard EM physics constructor.
  RegisterPhysics(new G4EmStandardPhysics_option4());

  // MicroElec Physics:
  // This physics list provides very detailed, low-energy electron transport models specifically
  // for Silicon. It is crucial for simulating the track structure and energy deposition
  // at the nanometer scale in the substrate.
  RegisterPhysics(new MicroElecPhysics());

  // Decay physics
  RegisterPhysics(new G4DecayPhysics());
}

PhysicsList::~PhysicsList()
{
}

void PhysicsList::ConstructParticle()
{
  G4VModularPhysicsList::ConstructParticle();
}

void PhysicsList::ConstructProcess()
{
  G4VModularPhysicsList::ConstructProcess();
}

void PhysicsList::SetCuts()
{
  SetCutsWithDefault();

  // Set low production cuts
  // The MicroElec physics list handles very low energy transport, but production cuts
  // for delta rays or photons can be relevant.
  SetCutValue(1.0*nm, "gamma");
  SetCutValue(1.0*nm, "e-");
  SetCutValue(1.0*nm, "e+");

  if (verboseLevel >0){
    DumpCutValuesTable();
  }
}
