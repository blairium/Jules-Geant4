#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"
#include "G4Track.hh"
#include "G4Step.hh"

class SensitiveDetector : public G4VSensitiveDetector
{
  public:
    SensitiveDetector(const G4String& name);
    virtual ~SensitiveDetector();

    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
};

#endif
