#include "BucketAnalysisManager.hh"

BucketAnalysisManager::BucketAnalysisManager(const G4String& fout){
    m_pFile = new TFile(fout, "recreate");
    m_pTree = new TTree("events", "The Bucket MC Events");
}

BucketAnalysisManager::~BucketAnalysisManager(){}
