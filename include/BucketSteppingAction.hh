#pragma once

#include "BucketAnalysisManager.hh"
#include <G4UserSteppingAction.hh>
#include <globals.hh>

class BucketSteppingAction : public G4UserSteppingAction {
    public:
        BucketSteppingAction(BucketAnalysisManager *);
        ~BucketSteppingAction();

        void UserSteppingAction(const G4Step* pStep) override;

    private:
        BucketAnalysisManager *m_pAnalysisManager;
};
