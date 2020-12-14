#include "BucketAnalysisManager.hh"

BucketAnalysisManager::BucketAnalysisManager(const G4String &fout) : ofs(fout, std::ios::trunc){
    ofs << "nref,nlambertian,nspike,nlobe,nback,time" << G4endl;
}

BucketAnalysisManager::~BucketAnalysisManager(){}
