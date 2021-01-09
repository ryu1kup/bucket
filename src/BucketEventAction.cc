#include "BucketEventAction.hh"
#include <G4Event.hh>

BucketEventAction::BucketEventAction(BucketAnalysisManager *pAnalysisManager) : m_pAnalysisManager(pAnalysisManager){}

BucketEventAction::~BucketEventAction(){}

void BucketEventAction::BeginOfEventAction(const G4Event *pEvent){
    m_pAnalysisManager->InitializeEvent();

    const auto iEventID = pEvent->GetEventID();
    if (iEventID % 9216 == 0) {
        G4cout << "Event: " << iEventID << G4endl;
    }
}

void BucketEventAction::EndOfEventAction(const G4Event *){
    m_pAnalysisManager->Write();
}
