#include "BucketActionInitialization.hh"
#include "BucketPrimaryGeneratorAction.hh"
#include "BucketEventAction.hh"
#include "BucketSteppingAction.hh"

BucketActionInitialization::BucketActionInitialization(const G4String &fout){
    m_pAnalysisManager = new BucketAnalysisManager(fout);
}

BucketActionInitialization::~BucketActionInitialization(){
    delete m_pAnalysisManager;
}

void BucketActionInitialization::Build() const {
    SetUserAction(new BucketPrimaryGeneratorAction());
    SetUserAction(new BucketEventAction(m_pAnalysisManager));
    SetUserAction(new BucketSteppingAction(m_pAnalysisManager));
}
