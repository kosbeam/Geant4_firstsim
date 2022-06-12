#include "physics.hh"

MyPhysicsList ::MyPhysicsList(/* args */)
{
    RegisterPhysics (new G4EmStandardPhysics());
    RegisterPhysics (new G4OpticalPhysics());
    RegisterPhysics(new G4DecayPhysics());
    RegisterPhysics(new G4RadioactiveDecayPhysics());

}

MyPhysicsList::~MyPhysicsList()
{
}