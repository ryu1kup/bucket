#pragma once

#include <G4UserSteppingAction.hh>
#include <globals.hh>

class BucketSteppingAction : public G4UserSteppingAction {
    public:
        BucketSteppingAction();
        ~BucketSteppingAction();

        void UserSteppingAction(const G4Step* pStep) override;
};
