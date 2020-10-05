#pragma once

#include "BucketAnalysisManager.hh"
#include <G4VUserActionInitialization.hh>

class BucketActionInitialization : public G4VUserActionInitialization {
    public:
        BucketActionInitialization(const G4String &);
        ~BucketActionInitialization();
        void Build() const override;

    private:
        BucketAnalysisManager *m_pAnalysisManager;
};
