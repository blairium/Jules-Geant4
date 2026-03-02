#include "RunAction.hh"
#include "G4Threading.hh" // For MT detection

RunAction::RunAction()
: G4UserRunAction()
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetDefaultFileType("root");
  analysisManager->SetFileName("microelec_output.root");

  // Configure Ntuple Merging:
  // When running in Multi-Threaded mode, Geant4 creates worker threads. By default,
  // each thread writes to a separate file (output_t0.root, output_t1.root, etc.).
  // This command merges all data into a single file (microelec_output.root) at the end of the run.
  analysisManager->SetNtupleMerging(true);

  // --- Output Data Structure Definition ---

  // Ntuple 0: "ElectronDistribution"
  // Used to store the spatial coordinates of electrons at every step inside the Si/SiO2 volumes.
  // This allows reconstructing the track structure or "cloud" of electrons.
  analysisManager->CreateNtuple("ElectronDistribution", "Spatial Distribution of Electrons");
  analysisManager->CreateNtupleDColumn("x"); // Position in nm
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("kineticEnergy"); // Kinetic energy in eV
  analysisManager->CreateNtupleIColumn("volumeID"); // ID to distinguish material: 0=Si, 1=SiO2
  analysisManager->FinishNtuple();

  // Ntuple 1: "DetectorHits"
  // Stores the position and energy of electrons specifically when they cross the
  // sensitive detector boundary located 1000nm above the sample.
  analysisManager->CreateNtuple("DetectorHits", "Electrons Detected");
  analysisManager->CreateNtupleDColumn("x");
  analysisManager->CreateNtupleDColumn("y");
  analysisManager->CreateNtupleDColumn("z");
  analysisManager->CreateNtupleDColumn("kineticEnergy");
  analysisManager->FinishNtuple();

  // Histogram 0: "EnergySpectrum"
  // A 1D histogram to bin the kinetic energies of detected electrons.
  // Range: 0 to 100 eV.
  // Binning: 2000 bins => 100 eV / 2000 = 0.05 eV per bin width.
  analysisManager->CreateH1("EnergySpectrum", "Secondary Electron Emission Spectrum", 2000, 0., 100.);
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run*)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("root");
  analysisManager->OpenFile("microelec_output.root");
}

void RunAction::EndOfRunAction(const G4Run*)
{
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();
}
