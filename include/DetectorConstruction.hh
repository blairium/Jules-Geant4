#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"
#include "G4Orb.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

  private:
    G4double fWorldSize;
    G4double fSubstrateSizeX;
    G4double fSubstrateSizeY;
    G4double fSubstrateSizeZ;
    G4double fOverlayerThickness;
    G4double fDetectorRadius;
    G4double fDetectorPosition;

    G4LogicalVolume* fLogicDetector;
};

#endif
