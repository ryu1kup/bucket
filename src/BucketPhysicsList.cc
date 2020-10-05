#include "BucketPhysicsList.hh"
#include <G4EmLivermorePhysics.hh>
#include <G4OpticalPhysics.hh>

BucketPhysicsList::BucketPhysicsList() : G4VModularPhysicsList() {
    RegisterPhysics(new G4EmLivermorePhysics());
    RegisterPhysics(new G4OpticalPhysics());
}

BucketPhysicsList::~BucketPhysicsList(){}

void BucketPhysicsList::SetCuts(){}
