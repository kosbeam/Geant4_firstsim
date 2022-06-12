#ifndef CONSTRUCTION_HH

#define CONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"
#include "G4GenericMessenger.hh"
#include "G4Tubs.hh"

#include "detector.hh"

class MyDetectorConstruction : public G4VUserDetectorConstruction
{

public:
    MyDetectorConstruction(/* args */);
    ~MyDetectorConstruction();

    G4LogicalVolume *GetScoringVolume() const { return fScoringVolume; }

    virtual G4VPhysicalVolume *Construct();

private:
    virtual void ConstructSDandField();


    G4int nCols, nRows;

    G4Box *solidWorld, *solidRadiator, *solidDetector; // 3 box

    G4Tubs *solidScintillator; // cylindrical geometry

    G4LogicalVolume *logicWorld, *logicRadiator, *logicDetector, *fScoringVolume, *logicScintillator;

    G4VPhysicalVolume *physWorld, *physDetector, *physScintillator;

    G4GenericMessenger *fMessenger;
    
    void DefineMaterial();

    void ConstructCherenkov();
    
    void ConstructScintillator();


    G4Material *SiO2, *H2O, *Aerogel, *worldMat, *NaI;

    G4Element *C;

    G4double xWorld, yWorld, zWorld;

    G4bool isCherenkov, isScintillator;
};

#endif