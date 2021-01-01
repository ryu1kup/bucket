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
            m_bIsDetected = false;
        }

        inline void IncrementNumReflection(){
            ++m_iNumReflection;
        }
        inline void IncrementNumLambertianReflection(){
            ++m_iNumLambertian;
        }
        inline void IncrementNumSpikeReflection(){
            ++m_iNumSpike;
        }
        inline void IncrementNumLobeReflection(){
            ++m_iNumLobe;
        }
        inline void IncrementNumBackscattering(){
            ++m_iNumBack;
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
        G4double m_dTime;
        G4bool m_bIsDetected;

        TFile* m_pFile;
        TTree* m_pTree;
};
