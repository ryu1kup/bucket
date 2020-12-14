#include "BucketDetectorConstruction.hh"
#include "BucketDetectorMessenger.hh"


BucketDetectorMessenger::BucketDetectorMessenger(BucketDetectorConstruction *pDetector) : m_pDetector(pDetector)
{
    m_pDirectory = new G4UIdirectory("/bucket/detector/");
    m_pDirectory->SetGuidance("DetectorConstruction control command");

    m_pSetePTFEReflectivity = new G4UIcmdWithADouble("/bucket/detector/eptfe_reflectivity", this);
    m_pSetePTFEReflectivity->SetGuidance("Set ePTFE Reflectivity");
    m_pSetePTFEReflectivity->SetDefaultValue(0.99);
    m_pSetePTFEReflectivity->AvailableForStates(G4State_PreInit);

    m_pSetePTFESpecularSpikeConstant = new G4UIcmdWithADouble("/bucket/detector/eptfe_specularspike", this);
    m_pSetePTFESpecularSpikeConstant->SetGuidance("Set ePTFE SpecularSpikeConstant");
    m_pSetePTFESpecularSpikeConstant->SetDefaultValue(0.0);
    m_pSetePTFESpecularSpikeConstant->AvailableForStates(G4State_PreInit);

    m_pSetePTFESpecularLobeConstant = new G4UIcmdWithADouble("/bucket/detector/eptfe_specularlobe", this);
    m_pSetePTFESpecularLobeConstant->SetGuidance("Set ePTFE SpecularLobeConstant");
    m_pSetePTFESpecularLobeConstant->SetDefaultValue(0.0);
    m_pSetePTFESpecularLobeConstant->AvailableForStates(G4State_PreInit);

    m_pSetePTFEBackscatteringConstant = new G4UIcmdWithADouble("/bucket/detector/eptfe_backscattering", this);
    m_pSetePTFEBackscatteringConstant->SetGuidance("Set ePTFE BackscatteringConstant");
    m_pSetePTFEBackscatteringConstant->SetDefaultValue(0.0);
    m_pSetePTFEBackscatteringConstant->AvailableForStates(G4State_PreInit);

    m_pSetSteelReflectivity = new G4UIcmdWithADouble("/bucket/detector/steel_reflectivity", this);
    m_pSetSteelReflectivity->SetGuidance("Set Steel Reflectivity");
    m_pSetSteelReflectivity->SetDefaultValue(0.90);
    m_pSetSteelReflectivity->AvailableForStates(G4State_PreInit);

    m_pSetWaterAbslength = new G4UIcmdWithADoubleAndUnit("/bucket/detector/water_abslength", this);
    m_pSetWaterAbslength->SetGuidance("Set Water Abslength");
    m_pSetWaterAbslength->SetDefaultValue(100.0);
    m_pSetWaterAbslength->SetDefaultUnit("m");
    m_pSetWaterAbslength->AvailableForStates(G4State_PreInit);
}

BucketDetectorMessenger::~BucketDetectorMessenger(){
    delete m_pSetePTFEReflectivity;
    delete m_pSetePTFESpecularSpikeConstant;
    delete m_pSetePTFESpecularLobeConstant;
    delete m_pSetePTFEBackscatteringConstant;
    delete m_pSetSteelReflectivity;
    delete m_pSetWaterAbslength;
    delete m_pDirectory;
}

void BucketDetectorMessenger::SetNewValue(G4UIcommand *pCommand, G4String hNewValue){
    if (pCommand == m_pSetePTFEReflectivity) {
        m_pDetector->SetePTFEReflectivity(m_pSetePTFEReflectivity->GetNewDoubleValue(hNewValue));
    } else if (pCommand == m_pSetePTFESpecularSpikeConstant) {
        m_pDetector->SetePTFESpecularSpikeConstant(m_pSetePTFESpecularSpikeConstant->GetNewDoubleValue(hNewValue));
    } else if (pCommand == m_pSetePTFESpecularLobeConstant) {
        m_pDetector->SetePTFESpecularLobeConstant(m_pSetePTFESpecularLobeConstant->GetNewDoubleValue(hNewValue));
    } else if (pCommand == m_pSetePTFEBackscatteringConstant) {
        m_pDetector->SetePTFEBackscatteringConstant(m_pSetePTFEBackscatteringConstant->GetNewDoubleValue(hNewValue));
    } else if (pCommand == m_pSetSteelReflectivity) {
        m_pDetector->SetSteelReflectivity(m_pSetSteelReflectivity->GetNewDoubleValue(hNewValue));
    } else if (pCommand == m_pSetWaterAbslength) {
        m_pDetector->SetWaterAbslength(m_pSetWaterAbslength->GetNewDoubleValue(hNewValue));
    }
}
