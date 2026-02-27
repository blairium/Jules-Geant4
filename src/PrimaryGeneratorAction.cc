#include "PrimaryGeneratorAction.hh"

#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0)
{
  G4int n_particle = 1;
  fParticleGun = new G4ParticleGun(n_particle);

  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName="gamma");
  fParticleGun->SetParticleDefinition(particle);

  // Normal incidence
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., -1.)); // Downwards

  // Energy
  fParticleGun->SetParticleEnergy(185.0 * eV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  // Position: Between detector and substrate.
  // Substrate top is at Z=0 (overlayer). Detector at Z=1000nm.
  // Beam starts "between" -> e.g., Z=500nm.
  G4double z0 = 500.0 * nm;

  // Pencil beam with diameter 10nm (radius 5nm)
  G4double radius = 5.0 * nm;
  G4double r = radius * std::sqrt(G4UniformRand()); // Uniform distribution in circle
  G4double phi = 2.0 * 3.14159265 * G4UniformRand();

  G4double x0 = r * std::cos(phi);
  G4double y0 = r * std::sin(phi);

  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

  fParticleGun->GeneratePrimaryVertex(anEvent);
}
