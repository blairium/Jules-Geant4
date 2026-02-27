#include "SteppingAction.hh"
#include "G4Track.hh"
#include "G4Electron.hh"
#include "G4AnalysisManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction()
: G4UserSteppingAction()
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
  G4Track* track = step->GetTrack();

  // We only care about electrons
  if (track->GetDefinition() != G4Electron::ElectronDefinition()) return;

  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4VPhysicalVolume* volume = preStepPoint->GetPhysicalVolume();

  if (!volume) return;

  G4String volName = volume->GetName();
  G4int volumeID = -1;

  if (volName == "Substrate") volumeID = 0; // Si
  else if (volName == "Overlayer") volumeID = 1; // SiO2

  if (volumeID != -1) {
    // Record position and energy
    G4ThreeVector pos = preStepPoint->GetPosition();
    G4double kinE = preStepPoint->GetKineticEnergy();

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, 0, pos.x() / nm);
    analysisManager->FillNtupleDColumn(0, 1, pos.y() / nm);
    analysisManager->FillNtupleDColumn(0, 2, pos.z() / nm);
    analysisManager->FillNtupleDColumn(0, 3, kinE / eV);
    analysisManager->FillNtupleIColumn(0, 4, volumeID);
    analysisManager->AddNtupleRow(0);
  }
}
