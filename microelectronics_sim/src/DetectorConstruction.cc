#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"
#include "G4RunManager.hh"

DetectorConstruction::DetectorConstruction()
: G4VUserDetectorConstruction(),
  fWorldSize(5.0 * um),
  fSubstrateSizeX(500.0 * nm),
  fSubstrateSizeY(500.0 * nm),
  fSubstrateSizeZ(500.0 * nm),
  fOverlayerThickness(1.0 * nm),
  fDetectorRadius(100.0 * nm), // Arbitrary for sensitive boundary
  fDetectorPosition(1000.0 * nm),
  fLogicDetector(nullptr)
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic");
  G4Material* silicon = nist->FindOrBuildMaterial("G4_Si");
  G4Material* sio2 = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  // World: The simulation volume encompassing all other volumes.
  // It uses a vacuum material (G4_Galactic).
  G4Box* solidWorld = new G4Box("World", fWorldSize / 2, fWorldSize / 2, fWorldSize / 2);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, vacuum, "World");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);

  // Silicon Substrate
  // We position the interface between Si and SiO2 at z=0.
  // The Silicon substrate extends downwards from z=0 to z=-500nm.
  G4Box* solidSubstrate = new G4Box("Substrate", fSubstrateSizeX / 2, fSubstrateSizeY / 2, fSubstrateSizeZ / 2);
  G4LogicalVolume* logicSubstrate = new G4LogicalVolume(solidSubstrate, silicon, "Substrate");
  new G4PVPlacement(0, G4ThreeVector(0, 0, -fSubstrateSizeZ / 2), logicSubstrate, "Substrate", logicWorld, false, 0);

  // SiO2 Overlayer
  // This 1nm layer sits directly on top of the silicon substrate.
  // It extends from z=0 to z=1nm.
  G4Box* solidOverlayer = new G4Box("Overlayer", fSubstrateSizeX / 2, fSubstrateSizeY / 2, fOverlayerThickness / 2);
  G4LogicalVolume* logicOverlayer = new G4LogicalVolume(solidOverlayer, sio2, "Overlayer");
  new G4PVPlacement(0, G4ThreeVector(0, 0, fOverlayerThickness / 2), logicOverlayer, "Overlayer", logicWorld, false, 0);

  // Detector
  // Implemented as a hemispherical shell centered at the origin (sample surface).
  // The inner radius is 1000nm, matching the requirement for detection "1000 nm above the sample".
  // This captures particles emitted upwards from the surface.
  // We use a thin shell (1nm thick) to act as a sensitive volume for detection.
  G4double detectorInnerRadius = fDetectorPosition;
  G4double detectorOuterRadius = fDetectorPosition + 1.0*nm;

  // Create a hemisphere (0 to 360 deg phi, 0 to 90 deg theta)
  G4Sphere* solidDetector = new G4Sphere("Detector", detectorInnerRadius, detectorOuterRadius, 0.*deg, 360.*deg, 0.*deg, 90.*deg);
  fLogicDetector = new G4LogicalVolume(solidDetector, vacuum, "Detector"); // Use vacuum material but make it sensitive

  // Place it. Centered at origin (sample surface).
  new G4PVPlacement(0, G4ThreeVector(0,0,0), fLogicDetector, "Detector", logicWorld, false, 0);

  // Visualization attributes
  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0)); // White
  visAttributes->SetVisibility(false);
  logicWorld->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0)); // Blue
  logicSubstrate->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0)); // Green
  logicOverlayer->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0)); // Red
  visAttributes->SetForceWireframe(true);
  fLogicDetector->SetVisAttributes(visAttributes);

  return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
  G4String detectorName = "ElectronDetector";
  SensitiveDetector* detector = new SensitiveDetector(detectorName);
  G4SDManager::GetSDMpointer()->AddNewDetector(detector);
  SetSensitiveDetector(fLogicDetector, detector);
}
