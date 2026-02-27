#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmMicroElecPhysics.hh"
#include "G4EmStandardPhysics_option4.hh" // Standard EM physics for higher energy/other particles
#include "G4EmLowEPPhysics.hh" // Low energy package if needed, but option4 is generally robust

PhysicsList::PhysicsList()
: G4VModularPhysicsList()
{
  SetVerboseLevel(1);

  // Standard EM Physics (Option 4 is accurate for low energies)
  // This handles the initial photon interaction (Photoelectric effect) and transport in vacuum/other materials
  RegisterPhysics(new G4EmStandardPhysics_option4());

  // MicroElec Physics - this handles the specific microdosimetry in Silicon
  // Note: G4EmMicroElecPhysics might override standard processes for specific particles in specific regions if configured correctly,
  // or it adds specific models. In many examples, it's used alongside standard EM.
  RegisterPhysics(new G4EmMicroElecPhysics());

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
