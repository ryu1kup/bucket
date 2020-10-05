#pragma once

#include <globals.hh>
#include <G4SystemOfUnits.hh>
#include <fstream>

class BucketAnalysisManager {
    public:
        BucketAnalysisManager(const G4String &fout);
        ~BucketAnalysisManager();

        inline void InitializeEvent(){
            m_dTime = -1;
            m_iNumReflection = 0;
            m_bIsDetected = false;
        }

        inline void IncrementNumReflection(){
            ++m_iNumReflection;
        }

        inline void DetectPhoton(const G4double dTime){
            m_dTime = dTime;
            m_bIsDetected = true;
        }

        inline void Write(){
            if (m_bIsDetected) {
                ofs << m_iNumReflection << "," << m_dTime / ns << G4endl;
            }
        }

    private:
        G4int m_iNumReflection;
        G4double m_dTime;
        G4bool m_bIsDetected;
        std::ofstream ofs;
};
