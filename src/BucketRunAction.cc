#include "BucketRunAction.hh"

BucketRunAction::BucketRunAction(BucketAnalysisManager* pAnalysisManager) : m_pAnalysisManager(pAnalysisManager){}

BucketRunAction::~BucketRunAction(){}

void BucketRunAction::BeginOfRunAction(const G4Run*){
    m_pAnalysisManager->InitializeRun();
}

void BucketRunAction::EndOfRunAction(const G4Run*){
    m_pAnalysisManager->TerminateRun();
}
