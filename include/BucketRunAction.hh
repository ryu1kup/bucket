#pragma once

#include "BucketAnalysisManager.hh"
#include <G4UserRunAction.hh>

class BucketRunAction : public G4UserRunAction {
    public:
        BucketRunAction(BucketAnalysisManager*);
        ~BucketRunAction();

        void BeginOfRunAction(const G4Run*) override;
        void EndOfRunAction(const G4Run*) override;

    private:
        BucketAnalysisManager *m_pAnalysisManager;
};
