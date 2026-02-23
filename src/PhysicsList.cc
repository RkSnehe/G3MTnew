#include "PhysicsList.hh"
#include "G4EmStandardPhysics.hh"
#include "G4DecayPhysics.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
    // Electromagnetic physics
    RegisterPhysics(new G4EmStandardPhysics());

    // Decay physics
    RegisterPhysics(new G4DecayPhysics());
}

PhysicsList::~PhysicsList() {}

