#include "SensitiveDetector.hh"
#include "G4Electron.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

SensitiveDetector::SensitiveDetector(const G4String& name)
: G4VSensitiveDetector(name)
{
}

SensitiveDetector::~SensitiveDetector()
{
}

G4bool SensitiveDetector::ProcessHits(G4Step* step, G4TouchableHistory*)
{
  G4Track* track = step->GetTrack();

  // We only care about electrons hitting the detector
  if (track->GetDefinition() != G4Electron::ElectronDefinition()) return false;

  // Get the energy and position upon entering the sensitive volume
  G4StepPoint* preStepPoint = step->GetPreStepPoint();
  G4double kinE = preStepPoint->GetKineticEnergy();
  G4ThreeVector pos = preStepPoint->GetPosition();

  // Store in Ntuple 1 (DetectorHits)
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(1, 0, pos.x() / nm);
  analysisManager->FillNtupleDColumn(1, 1, pos.y() / nm);
  analysisManager->FillNtupleDColumn(1, 2, pos.z() / nm);
  analysisManager->FillNtupleDColumn(1, 3, kinE / eV);
  analysisManager->AddNtupleRow(1);

  // Fill Histogram for Energy Spectrum
  analysisManager->FillH1(0, kinE / eV);

  // Kill the track after detection so it doesn't get counted again or continue
  track->SetTrackStatus(fStopAndKill);

  return true;
}
