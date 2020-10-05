#pragma once

#include <G4VModularPhysicsList.hh>

class BucketPhysicsList : public G4VModularPhysicsList
{
    public:
        BucketPhysicsList();
        ~BucketPhysicsList();

    public:
        void SetCuts() override;
};
