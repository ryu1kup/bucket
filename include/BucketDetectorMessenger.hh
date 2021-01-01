#pragma once

#include <G4UImessenger.hh>
#include <G4UIcommand.hh>
#include <G4UIdirectory.hh>
#include <G4UIcmdWithADouble.hh>
#include <G4UIcmdWithADoubleAndUnit.hh>

class BucketDetectorConstruction;

class BucketDetectorMessenger : public G4UImessenger {
    public:
        BucketDetectorMessenger(BucketDetectorConstruction *pDetector);
        ~BucketDetectorMessenger();

        void SetNewValue(G4UIcommand *pCommand, G4String hNewValue);

    private:
        BucketDetectorConstruction *m_pDetector;
        G4UIdirectory *m_pDirectory;
        G4UIcmdWithADouble *m_pSetePTFEReflectivity;
        G4UIcmdWithADouble *m_pSetePTFESpecularSpikeConstant;
        G4UIcmdWithADouble *m_pSetePTFESpecularLobeConstant;
        G4UIcmdWithADouble *m_pSetePTFEBackscatteringConstant;
        G4UIcmdWithADouble *m_pSetePTFESigmaAlpha;
        G4UIcmdWithADouble *m_pSetSteelReflectivity;
        G4UIcmdWithADoubleAndUnit *m_pSetWaterAbslength;
};
