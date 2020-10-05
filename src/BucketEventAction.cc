#include "BucketEventAction.hh"

BucketEventAction::BucketEventAction(BucketAnalysisManager *pAnalysisManager) : m_pAnalysisManager(pAnalysisManager){}

BucketEventAction::~BucketEventAction(){}

void BucketEventAction::BeginOfEventAction(const G4Event *){
    m_pAnalysisManager->InitializeEvent();
}

void BucketEventAction::EndOfEventAction(const G4Event *){
    m_pAnalysisManager->Write();
}
