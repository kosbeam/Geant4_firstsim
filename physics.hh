#ifndef PHYSICS_HH

#define PHYSICS_HH

#include "G4VModularPhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"

// #include "G4VUserDetectorConstruction.hh"
// #include "G4VPhysicalVolume.hh"
// #include "G4LogicalVolume.hh"
// #include "G4Box.hh"
// #include "G4PVPlacement.hh"
// #include "G4NistManager.hh"
// #include "G4SystemOfUnits.hh"

class MyPhysicsList : public G4VModularPhysicsList
{
    public :
    MyPhysicsList(/* args */);
    ~MyPhysicsList();
};



#endif