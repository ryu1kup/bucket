#include "BucketAnalysisManager.hh"

BucketAnalysisManager::BucketAnalysisManager(const G4String &fout) : ofs(fout, std::ios::trunc){
    ofs << "nref,time" << G4endl;
}

BucketAnalysisManager::~BucketAnalysisManager(){}
