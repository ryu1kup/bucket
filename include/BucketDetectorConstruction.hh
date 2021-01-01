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
        void SetePTFESigmaAlpha(G4double value) {m_pMaterials->SetePTFESigmaAlpha(value);}
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
        G4LogicalVolume *m_pReflectorLidForHoleLogicalVolume;
        G4LogicalVolume *m_pWaterLogicalVolume;
        G4LogicalVolume *m_pInjectorHeadFrontLogicalVolume;
        G4LogicalVolume *m_pInjectorHeadMiddleLogicalVolume;
        G4LogicalVolume *m_pInjectorHeadBackLogicalVolume;
        G4LogicalVolume *m_pInjectorFiberCase1LogicalVolume;
        G4LogicalVolume *m_pInjectorFiberCaseNut1LogicalVolume;
        G4LogicalVolume *m_pInjectorFiberCase2LogicalVolume;
        G4LogicalVolume *m_pInjectorFiberCaseNut2LogicalVolume;
        G4LogicalVolume *m_pInjectorFiberCaseNut3LogicalVolume;
        G4LogicalVolume *m_pInjectorHeadNut1LogicalVolume;
        G4LogicalVolume *m_pInjectorHeadNut2LogicalVolume;
        G4LogicalVolume *m_pInjectorHeadNut3LogicalVolume;

        G4VPhysicalVolume *m_pWorldPhysicalVolume;
        G4VPhysicalVolume *m_pBucketSidePhysicalVolume;
        G4VPhysicalVolume *m_pBucketBottomPhysicalVolume;
        G4VPhysicalVolume *m_pBucketLidPhysicalVolume;
        G4VPhysicalVolume *m_pReflectorSidePhysicalVolume;
        G4VPhysicalVolume *m_pReflectorBottomPhysicalVolume;
        G4VPhysicalVolume *m_pReflectorLidPhysicalVolume;
        G4VPhysicalVolume *m_pReflectorLidForHolePhysicalVolume;
        G4VPhysicalVolume *m_pWaterPhysicalVolume;
        G4VPhysicalVolume *m_pInjectorHeadFrontPhysicalVolume;
        G4VPhysicalVolume *m_pInjectorHeadMiddlePhysicalVolume;
        G4VPhysicalVolume *m_pInjectorHeadBackPhysicalVolume;
        G4VPhysicalVolume *m_pInjectorFiberCase1PhysicalVolume;
        G4VPhysicalVolume *m_pInjectorFiberCaseNut1PhysicalVolume;
        G4VPhysicalVolume *m_pInjectorFiberCase2PhysicalVolume;
        G4VPhysicalVolume *m_pInjectorFiberCaseNut2PhysicalVolume;
        G4VPhysicalVolume *m_pInjectorFiberCaseNut3PhysicalVolume;
        G4VPhysicalVolume *m_pInjectorHeadNut1PhysicalVolume[6];
        G4VPhysicalVolume *m_pInjectorHeadNut2PhysicalVolume[6];
        G4VPhysicalVolume *m_pInjectorHeadNut3PhysicalVolume[6];

        G4LogicalBorderSurface *m_pBorderWater2ReflectorSide;
        G4LogicalBorderSurface *m_pBorderWater2ReflectorBottom;
        G4LogicalBorderSurface *m_pBorderWater2ReflectorLid;
        G4LogicalBorderSurface *m_pBorderWater2ReflectorLidForHole;
        G4LogicalBorderSurface *m_pBorderWater2PMTAluminiumCover;
        G4LogicalBorderSurface *m_pBorderWater2InjectorHeadFront;
        G4LogicalBorderSurface *m_pBorderWater2InjectorHeadMiddle;
        G4LogicalBorderSurface *m_pBorderWater2InjectorHeadBack;
        G4LogicalBorderSurface *m_pBorderWater2InjectorFiberCase1;
        G4LogicalBorderSurface *m_pBorderWater2InjectorFiberCaseNut1;
        G4LogicalBorderSurface *m_pBorderWater2InjectorFiberCase2;
        G4LogicalBorderSurface *m_pBorderWater2InjectorFiberCaseNut2;
        G4LogicalBorderSurface *m_pBorderWater2InjectorFiberCaseNut3;
        G4LogicalBorderSurface *m_pBorderWater2InjectorHeadNut1[6];
        G4LogicalBorderSurface *m_pBorderWater2InjectorHeadNut2[6];
        G4LogicalBorderSurface *m_pBorderWater2InjectorHeadNut3[6];

        BucketMaterials *m_pMaterials;
        BucketDetectorMessenger *m_pMessenger;
};
