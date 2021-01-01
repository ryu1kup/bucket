#pragma once

#include <G4Material.hh>

class BucketMaterials {
    public:
        BucketMaterials();
        ~BucketMaterials();

        void SetePTFEReflectivity(const G4double value) {m_dePTFEReflectivity = value;}
        void SetePTFESpecularSpikeConstant(const G4double value) {m_dePTFESpecularSpikeConstant = value;}
        void SetePTFESpecularLobeConstant(const G4double value) {m_dePTFESpecularLobeConstant = value;}
        void SetePTFEBackscatteringConstant(const G4double value) {m_dePTFEBackscatteringConstant = value;}
        void SetePTFESigmaAlpha(const G4double value) {m_dePTFESigmaAlpha = value;}
        void SetSteelReflectivity(const G4double value) {m_dSteelReflectivity = value;}
        void SetWaterAbslength(const G4double value) {m_dWaterAbslength = value;}

        G4double ePTFESigmaAlpha() {return m_dePTFESigmaAlpha;}

    public:
        void DefineMaterials() const;
        G4double m_dePTFEReflectivity;
        G4double m_dePTFESpecularSpikeConstant;
        G4double m_dePTFESpecularLobeConstant;
        G4double m_dePTFEBackscatteringConstant;
        G4double m_dePTFESigmaAlpha;
        G4double m_dSteelReflectivity;
        G4double m_dWaterAbslength;
};
