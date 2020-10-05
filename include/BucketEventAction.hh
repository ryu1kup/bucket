#pragma once

#include "BucketAnalysisManager.hh"
#include <G4UserEventAction.hh>

class BucketEventAction : public G4UserEventAction {
    public:
        BucketEventAction(BucketAnalysisManager *);
        ~BucketEventAction();

        void BeginOfEventAction(const G4Event *) override;
        void EndOfEventAction(const G4Event *) override;

    private:
        BucketAnalysisManager *m_pAnalysisManager;
};
