#pragma once

#include <globals.hh>
#include <G4SystemOfUnits.hh>
#include <iostream>
#include <TFile.h>
#include <TTree.h>

class BucketAnalysisManager {
    public:
        BucketAnalysisManager(const G4String &fout);
        ~BucketAnalysisManager();

        inline void InitializeRun(){
            m_pTree->Branch("nref", &m_iNumReflection, "nref/I");
            m_pTree->Branch("ndiffuse", &m_iNumLambertian, "ndiffuse/I");
            m_pTree->Branch("nspike", &m_iNumSpike, "nspike/I");
            m_pTree->Branch("nlobe", &m_iNumLobe, "nlobe/I");
            m_pTree->Branch("nback", &m_iNumBack, "nback/I");
            m_pTree->Branch("nfresnel", &m_iNumFresnelReflection, "nfresnel/I");
            m_pTree->Branch("ntotal", &m_iNumTotalInternalReflection, "ntotal/I");
            m_pTree->Branch("time", &m_dTime, "time/D");
        }

        inline void TerminateRun(){
            m_pTree->Write();
            m_pFile->Close();
        }

        inline void InitializeEvent(){
            m_dTime = -1;
            m_iNumReflection = 0;
            m_iNumLambertian = 0;
            m_iNumSpike = 0;
            m_iNumLobe = 0;
            m_iNumBack = 0;
            m_iNumTotalInternalReflection = 0;
            m_iNumFresnelReflection = 0;
            m_bIsDetected = false;
        }

        inline void IncrementNumReflection(){
            ++m_iNumReflection;
            ++m_iNumReflection;
        }
        inline void IncrementNumLambertianReflection(){
            ++m_iNumLambertian;
            ++m_iNumReflection;
        }
        inline void IncrementNumSpikeReflection(){
            ++m_iNumSpike;
            ++m_iNumReflection;
        }
        inline void IncrementNumLobeReflection(){
            ++m_iNumLobe;
            ++m_iNumReflection;
        }
        inline void IncrementNumBackscattering(){
            ++m_iNumBack;
            ++m_iNumReflection;
        }
        inline void IncrementNumFresnelReflection(){
            ++m_iNumFresnelReflection;
            ++m_iNumReflection;
        }
        inline void IncrementNumTotalInternalReflection(){
            ++m_iNumTotalInternalReflection;
            ++m_iNumReflection;
        }

        inline void DetectPhoton(const G4double dTime){
            m_dTime = dTime;
            m_bIsDetected = true;
        }

        inline void Write(){
            if (m_bIsDetected) {
                m_pTree->Fill();
            }
        }

    private:
        G4int m_iNumReflection;
        G4int m_iNumLambertian;
        G4int m_iNumSpike;
        G4int m_iNumLobe;
        G4int m_iNumBack;
        G4int m_iNumFresnelReflection;
        G4int m_iNumTotalInternalReflection;
        G4double m_dTime;
        G4bool m_bIsDetected;

        TFile* m_pFile;
        TTree* m_pTree;
};
