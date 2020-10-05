#pragma once

#include <G4VUserActionInitialization.hh>

class BucketActionInitialization : public G4VUserActionInitialization {
    public:
        BucketActionInitialization();
        ~BucketActionInitialization();
        void Build() const override;
};
