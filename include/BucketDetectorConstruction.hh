#pragma once

#include "BucketMaterials.hh"
#include <G4VUserDetectorConstruction.hh>
#include <G4LogicalVolume.hh>
#include <G4VPhysicalVolume.hh>
#include <G4LogicalBorderSurface.hh>
#include <vector>

class BucketDetectorMessenger;

class BucketDetectorConstruction : public G4VUserDetectorConstruction {
    public:
        BucketDetectorConstruction();
        ~BucketDetectorConstruction();

    public:
        G4VPhysicalVolume *Construct() override;
        void ConstructBucket();
        void ConstructLaserInjector(G4double x, G4double y, G4double z);
        void ConstructLogicalBorderSurfaces();

        void SetePTFEReflectivity(G4double value) {m_pMaterials->SetePTFEReflectivity(value);}
        void SetePTFESpecularSpikeConstant(G4double value) {m_pMaterials->SetePTFESpecularSpikeConstant(value);}
        void SetePTFESpecularLobeConstant(G4double value) {m_pMaterials->SetePTFESpecularLobeConstant(value);}
        void SetePTFEBackscatteringConstant(G4double value) {m_pMaterials->SetePTFEBackscatteringConstant(value);}
        void SetSteelReflectivity(G4double value) {m_pMaterials->SetSteelReflectivity(value);}
        void SetWaterAbslength(G4double value) {m_pMaterials->SetWaterAbslength(value);}

    private:
        G4LogicalVolume *m_pWorldLogicalVolume;
        G4LogicalVolume *m_pBucketSideLogicalVolume;
        G4LogicalVolume *m_pBucketBottomLogicalVolume;
        G4LogicalVolume *m_pBucketLidLogicalVolume;
        G4LogicalVolume *m_pReflectorSideLogicalVolume;
        G4LogicalVolume *m_pReflectorBottomLogicalVolume;
        G4LogicalVolume *m_pReflectorLidLogicalVolume;
        G4LogicalVolume *m_pWaterLogicalVolume;
        G4LogicalVolume *m_pPMTCaseLogicalVolume;
        G4LogicalVolume *m_pPMTWindowLogicalVolume;
        G4LogicalVolume *m_pPMTLogicalVolume;
        G4LogicalVolume *m_pFiberCaseLogicalVolume;
        G4LogicalVolume *m_pInjectorBun1LogicalVolume;
        G4LogicalVolume *m_pInjectorBun2LogicalVolume;
        G4LogicalVolume *m_pInjectorPattyLogicalVolume;

        G4VPhysicalVolume *m_pWorldPhysicalVolume;
        G4VPhysicalVolume *m_pBucketSidePhysicalVolume;
        G4VPhysicalVolume *m_pBucketBottomPhysicalVolume;
        G4VPhysicalVolume *m_pBucketLidPhysicalVolume;
        G4VPhysicalVolume *m_pReflectorSidePhysicalVolume;
        G4VPhysicalVolume *m_pReflectorBottomPhysicalVolume;
        G4VPhysicalVolume *m_pReflectorLidPhysicalVolume;
        G4VPhysicalVolume *m_pWaterPhysicalVolume;
        G4VPhysicalVolume *m_pPMT1CasePhysicalVolume;
        G4VPhysicalVolume *m_pPMT1WindowPhysicalVolume;
        G4VPhysicalVolume *m_pPMT1PhysicalVolume;
        G4VPhysicalVolume *m_pPMT2CasePhysicalVolume;
        G4VPhysicalVolume *m_pPMT2WindowPhysicalVolume;
        G4VPhysicalVolume *m_pPMT2PhysicalVolume;
        G4VPhysicalVolume *m_pFiberCasePhysicalVolume;
        G4VPhysicalVolume *m_pInjectorBun1PhysicalVolume;
        G4VPhysicalVolume *m_pInjectorBun2PhysicalVolume;
        G4VPhysicalVolume *m_pInjectorPattyPhysicalVolume;

        G4LogicalBorderSurface *m_pBorderWater2ReflectorSide;
        G4LogicalBorderSurface *m_pBorderWater2ReflectorBottom;
        G4LogicalBorderSurface *m_pBorderWater2ReflectorLid;
        G4LogicalBorderSurface *m_pBorderWater2FiberCase;
        G4LogicalBorderSurface *m_pBorderWater2InjectorBun1;
        G4LogicalBorderSurface *m_pBorderWater2InjectorBun2;
        G4LogicalBorderSurface *m_pBorderWater2InjectorPatty;

        BucketMaterials *m_pMaterials;
        BucketDetectorMessenger *m_pMessenger;
};
