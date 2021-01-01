#include "BucketRunAction.hh"
#include <G4Run.hh>

BucketRunAction::BucketRunAction(BucketAnalysisManager* pAnalysisManager) : m_pAnalysisManager(pAnalysisManager){}

BucketRunAction::~BucketRunAction(){}

void BucketRunAction::BeginOfRunAction(const G4Run*){
    m_pAnalysisManager->InitializeRun();

    G4cout << G4endl;
    G4cout << "**********************************************" << G4endl;
    G4cout << "*                                            *" << G4endl;
    G4cout << "*          welcome to the Bucket MC          *" << G4endl;
    G4cout << "*                                            *" << G4endl;
    G4cout << "**********************************************" << G4endl;
    G4cout << G4endl;
    G4cout << "----------------------------------------------" << G4endl;
}

void BucketRunAction::EndOfRunAction(const G4Run* pRun){
    m_pAnalysisManager->TerminateRun();

    G4cout << "Event: " << pRun->GetNumberOfEvent() << " -- finish!" << G4endl;
    G4cout << "----------------------------------------------" << G4endl;
    G4cout << G4endl;
    G4cout << "The Bucket MC has been finished without clash." << G4endl;
    G4cout << G4endl;
}
