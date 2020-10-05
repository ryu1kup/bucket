#pragma once

#include <G4UserEventAction.hh>

class BucketEventAction : public G4UserEventAction {
    public:
        BucketEventAction();
        ~BucketEventAction();

        void BeginOfEventAction(const G4Event *) override;
        void EndOfEventAction(const G4Event *) override;
};
