#include "RunAction.hh"
#include "G4Threading.hh" // For MT detection

RunAction::RunAction()
: G4UserRunAction()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFileName("microelec_output");

  // Enable merging of output files for multithreading
  // This avoids separate files for each worker thread (e.g., _t0.root, _t1.root)
  // if built with GEANT4_BUILD_MULTITHREADED=ON
  // Note: Only works for Root output format.
  analysisManager->SetNtupleMerging(true);

  // Create Ntuples/Trees

  // Ntuple 0: Spatial Distribution Inside Si/SiO2
  analysisManager->CreateNtuple("ElectronDistribution", "Spatial Distribution of Electrons");
  analysisManager->CreateNtupleDColumn("x"); // Position in nm
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("kineticEnergy"); // eV
  analysisManager->CreateNtupleIColumn("volumeID"); // 0=Si, 1=SiO2
  analysisManager->FinishNtuple();

  // Ntuple 1: Detector Hits
  analysisManager->CreateNtuple("DetectorHits", "Electrons Detected");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("kineticEnergy");
  analysisManager->FinishNtuple();

  // Histogram for Energy Spectrum
  // Range 0-100 eV, binning 0.05 eV -> 2000 bins
  analysisManager->CreateH1("EnergySpectrum", "Secondary Electron Emission Spectrum", 2000, 0., 100.);
}

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
