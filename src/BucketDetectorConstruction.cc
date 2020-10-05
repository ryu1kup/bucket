#include "BucketDetectorConstruction.hh"
#include "BucketMaterials.hh"
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4Transform3D.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>
#include <G4VisAttributes.hh>

BucketDetectorConstruction::BucketDetectorConstruction() : m_pMaterials(nullptr) {
    m_pMaterials = new BucketMaterials();
}

BucketDetectorConstruction::~BucketDetectorConstruction()
{
    delete m_pMaterials;
}

G4VPhysicalVolume* BucketDetectorConstruction::Construct() {
    m_pMaterials->DefineMaterials();

    constexpr G4double dWorldX = 5.0 * m;
    constexpr G4double dWorldY = 5.0 * m;
    constexpr G4double dWorldZ = 5.0 * m;
    auto *pWorldBox = new G4Box("WorldBox", dWorldX/2., dWorldY/2, dWorldZ/2.);

    auto *Air = G4Material::GetMaterial("Air");
    m_pWorldLogicalVolume = new G4LogicalVolume(pWorldBox, Air, "WorldLogicalVolume");
    m_pWorldLogicalVolume->SetVisAttributes(G4VisAttributes::Invisible);

    m_pWorldPhysicalVolume = new G4PVPlacement(G4Transform3D(), "World", m_pWorldLogicalVolume, 0, false, 0);

    // construct the bucket and including volumes
    ConstructBucket();

    return m_pWorldPhysicalVolume;
}

void BucketDetectorConstruction::ConstructBucket(){
    // construct volumes
    // bucket side
    constexpr G4double dBucketThickness = 10 * mm;
    constexpr G4double dBucketSideInnerRadius = 150 * mm;
    constexpr G4double dBucketSideOuterRadius = dBucketSideInnerRadius + dBucketThickness;
    constexpr G4double dBucketSideHalfZ = 150 * mm;
    auto *pBucketSideTubs = new G4Tubs("BucketSideTubs", dBucketSideInnerRadius, dBucketSideOuterRadius, dBucketSideHalfZ, 0, twopi);

    auto *SS304L = G4Material::GetMaterial("SS304L");
    m_pBucketSideLogicalVolume = new G4LogicalVolume(pBucketSideTubs, SS304L, "BucketSideLogicalVolume");

    m_pBucketSidePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "BucketSide", m_pBucketSideLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // bucket bottom
    constexpr G4double dBucketBottomOuterRadius = dBucketSideOuterRadius;
    constexpr G4double dBucketBottomHalfZ = 0.5 * dBucketThickness;
    auto *pBucketBottomTubs = new G4Tubs("BucketBottomTubs", 0, dBucketBottomOuterRadius, dBucketBottomHalfZ, 0, twopi);

    m_pBucketBottomLogicalVolume = new G4LogicalVolume(pBucketBottomTubs, SS304L, "BucketBottomLogicalVolume");

    constexpr G4double dBucketBottomOffsetZ = -dBucketSideHalfZ - dBucketBottomHalfZ;
    m_pBucketBottomPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dBucketBottomOffsetZ), "BucketBottom", m_pBucketBottomLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // bucket lid
    constexpr G4double dBucketLidThickness = 2 * mm;
    constexpr G4double dBucketLidOuterRadius = dBucketSideOuterRadius;
    constexpr G4double dBucketLidHalfZ = 0.5 * dBucketLidThickness;
    auto *pBucketLidTubs = new G4Tubs("BucketLidTubs", 0, dBucketLidOuterRadius, dBucketLidHalfZ, 0, twopi);

    auto *Aluminium = G4Material::GetMaterial("Aluminium");
    m_pBucketLidLogicalVolume = new G4LogicalVolume(pBucketLidTubs, Aluminium, "BucketLidLogicalVolume");

    constexpr G4double dBucketLidOffsetZ = dBucketSideHalfZ + dBucketLidHalfZ;
    m_pBucketLidPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dBucketLidOffsetZ), "BucketLid", m_pBucketLidLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // reflector side
    constexpr G4double dReflectorThickness = 1.5 * mm;
    constexpr G4double dReflectorSideOuterRadius = dBucketSideInnerRadius;
    constexpr G4double dReflectorSideInnerRadius = dReflectorSideOuterRadius - dReflectorThickness;
    constexpr G4double dReflectorSideHalfZ = dBucketSideHalfZ;
    auto *pReflectorSideTubs = new G4Tubs("ReflectorSideTubs", dReflectorSideInnerRadius, dReflectorSideOuterRadius, dReflectorSideHalfZ, 0, twopi);

    auto *ePTFE = G4Material::GetMaterial("ePTFE");
    m_pReflectorSideLogicalVolume = new G4LogicalVolume(pReflectorSideTubs, ePTFE, "ReflectorSideLogicalVolume");

    m_pReflectorSidePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "ReflectorSide", m_pReflectorSideLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // reflector bottom
    constexpr G4double dReflectorBottomOuterRadius = dReflectorSideInnerRadius;
    constexpr G4double dReflectorBottomHalfZ = 0.5 * dReflectorThickness;
    auto *pReflectorBottomTubs = new G4Tubs("ReflectorBottomTubs", 0, dReflectorBottomOuterRadius, dReflectorBottomHalfZ, 0, twopi);

    m_pReflectorBottomLogicalVolume = new G4LogicalVolume(pReflectorBottomTubs, ePTFE, "ReflectorBottomLogicalVolume");

    constexpr G4double dReflectorBottomOffsetZ = -dReflectorSideHalfZ + 0.5 * dReflectorThickness;
    m_pReflectorBottomPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dReflectorBottomOffsetZ), "ReflectorBottom", m_pReflectorBottomLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // reflector ild
    constexpr G4double dReflectorLidOuterRadius = dReflectorSideInnerRadius;
    constexpr G4double dReflectorLidHalfZ = 0.5 * dReflectorThickness;
    auto *pReflectorLidTubs = new G4Tubs("ReflectorLidTubs", 0, dReflectorLidOuterRadius, dReflectorLidHalfZ, 0, twopi);

    m_pReflectorLidLogicalVolume = new G4LogicalVolume(pReflectorLidTubs, ePTFE, "ReflectorLidLogicalVolume");

    constexpr G4double dReflectorLidOffsetZ = dReflectorSideHalfZ - 0.5 * dReflectorThickness;
    m_pReflectorLidPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dReflectorLidOffsetZ), "ReflectorLid", m_pReflectorLidLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // water
    constexpr G4double dWaterOuterRadius = dReflectorSideInnerRadius;
    constexpr G4double dWaterHalfZ = dReflectorSideHalfZ - dReflectorThickness;
    auto *pWaterTubs = new G4Tubs("WaterTubs", 0, dWaterOuterRadius, dWaterHalfZ, 0, twopi);

    auto *Water = G4Material::GetMaterial("Water");
    m_pWaterLogicalVolume = new G4LogicalVolume(pWaterTubs, Water, "WaterLogicalVolume");

    m_pWaterPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "Water", m_pWaterLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // PMT
    constexpr G4double dPMTCaseOuterRadius = 30 * mm;
    constexpr G4double dPMTCaseInnerRadius = 26.5 * mm;
    constexpr G4double dPMTCaseHalfZ = 0.5 * mm;
    auto *pPMTCaseTubs = new G4Tubs("PMTCaseTubs", dPMTCaseInnerRadius, dPMTCaseOuterRadius, dPMTCaseHalfZ, 0, twopi);

    auto *PMTCaseMaterial = G4Material::GetMaterial("PMTCaseMaterial");
    m_pPMTCaseLogicalVolume = new G4LogicalVolume(pPMTCaseTubs, PMTCaseMaterial, "PMTCaseLogicalVolume");

    constexpr G4double dPMT1CaseOffsetX = 50 * mm;
    constexpr G4double dPMT1CaseOffsetZ = dWaterHalfZ - dPMTCaseHalfZ;
    m_pPMT1CasePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dPMT1CaseOffsetX, 0, dPMT1CaseOffsetZ), "PMT1Case", m_pPMTCaseLogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    // TODO: PMT Window
    // constexpr G4double dPMTWindowThickness = 1 * mm;
    // constexpr G4double dPMTWindowOuterRadius = dPMTCaseInnerRadius;
    // constexpr G4double dPMTWindowHalfZ = 0.5 * dPMTWindowThickness;
    // auto *pPMTWindowTubs = new G4Tubs("PMTWindowTubs", 0, dPMTWindowOuterRadius, dPMTWindowHalfZ, 0, twopi);

    // auto *Glass = G4Material::GetMaterial("Glass");
    // m_pPMTWindowLogicalVolume = new G4LogicalVolume(pPMTWindowTubs, Glass, "PMTWindowLogialVolume");

    // constexpr G4double dPMT1WindowOffsetX = dPMT1CaseOffsetX;
    // constexpr G4double dPMT1WindowOffsetZ = dPMT1CaseOffsetZ;
    // m_pPMT1WindowLogicalVolume = new G4LogicalVolume(0, G4ThreeVector(dPMT1WindowOffsetX))

    constexpr G4double dPMTOuterRadius = dPMTCaseInnerRadius;
    constexpr G4double dPMTHalfZ = dPMTCaseHalfZ;
    auto *pPMTTubs = new G4Tubs("PMTTubs", 0, dPMTOuterRadius, dPMTHalfZ, 0, twopi);

    auto *Glass = G4Material::GetMaterial("Glass");
    m_pPMTLogicalVolume = new G4LogicalVolume(pPMTTubs, Glass, "PMTLogicalVolume");

    constexpr G4double dPMT1OffsetX = 50 * mm;
    constexpr G4double dPMT1OffsetZ = dPMT1CaseOffsetZ;
    m_pPMT1PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dPMT1OffsetX, 0, dPMT1OffsetZ), "PMT1", m_pPMTLogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    // second PMT
    constexpr G4bool construct_second_pmt = false;
    if (construct_second_pmt) {
        constexpr G4double dPMT2CaseOffsetX = dPMT1CaseOffsetX;
        constexpr G4double dPMT2CaseOffsetY = 90 * mm;
        constexpr G4double dPMT2CaseOffsetZ = dPMT1CaseOffsetZ;
        m_pPMT2CasePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dPMT2CaseOffsetX, dPMT2CaseOffsetY, dPMT2CaseOffsetZ), "PMT2Case", m_pPMTCaseLogicalVolume, m_pWaterPhysicalVolume, false, 1, true);

        constexpr G4double dPMT2OffsetX = dPMT1OffsetX; 
        constexpr G4double dPMT2OffsetY = 90 * mm; 
        constexpr G4double dPMT2OffsetZ = dPMT1OffsetZ;
        m_pPMT2PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dPMT2OffsetX, dPMT2OffsetY, dPMT2OffsetZ), "PMT2", m_pPMTLogicalVolume, m_pWaterPhysicalVolume, false, 1, true);
    }

    // fiber case (just a tube)
    constexpr G4double dFiberCaseOuterRadius = 1.5 * mm;
    constexpr G4double dFiberCaseHalfZ = 5 * mm;
    auto *pFiberCase = new G4Tubs("FiberCaseTubs", 0, dFiberCaseOuterRadius, dFiberCaseHalfZ, 0, twopi);

    m_pFiberCaseLogicalVolume = new G4LogicalVolume(pFiberCase, SS304L, "FiberCaseLogicalVolume");

    constexpr G4double dFiberCaseOffsetX = -50 * mm;
    constexpr G4double dFiberCaseOffsetZ = dWaterHalfZ - dFiberCaseHalfZ;
    m_pFiberCasePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dFiberCaseOffsetX, 0, dFiberCaseOffsetZ), "FiberCase", m_pFiberCaseLogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    // laser injector
    constexpr G4double dInjectorBun1OuterRadius = 18 * mm;
    constexpr G4double dInjectorBun1HalfZ = 3 * mm;
    auto *pInjectorBun1Tubs = new G4Tubs("InjectorBun1Tubs", 0, dInjectorBun1OuterRadius, dInjectorBun1HalfZ, 0, twopi);

    m_pInjectorBun1LogicalVolume = new G4LogicalVolume(pInjectorBun1Tubs, SS304L, "InjectorBun1LogicalVolume");

    constexpr G4double dInjectorBun1OffsetX = dFiberCaseOffsetX;
    constexpr G4double dInjectorBun1OffsetZ = dFiberCaseOffsetZ - dFiberCaseHalfZ - dInjectorBun1HalfZ;
    m_pInjectorBun1PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dInjectorBun1OffsetX, 0, dInjectorBun1OffsetZ), "InjectorBun1", m_pInjectorBun1LogicalVolume, m_pWaterPhysicalVolume, false, 0, true);
    constexpr G4double dInjectorPattyOuterRadius = dInjectorBun1OuterRadius;
    constexpr G4double dInjectorPattyHalfZ = 1.5 * mm;
    auto *pInjectorPattyTubs = new G4Tubs("InjectorPattyTubs", 0, dInjectorPattyOuterRadius, dInjectorPattyHalfZ, 0, twopi);

    m_pInjectorPattyLogicalVolume = new G4LogicalVolume(pInjectorPattyTubs, SS304L, "InjectorPattyLogicalVolume");

    constexpr G4double dInjectorPattyOffsetX = dFiberCaseOffsetX;
    constexpr G4double dInjectorPattyOffsetZ = dInjectorBun1OffsetZ - dInjectorBun1HalfZ - dInjectorPattyHalfZ;
    m_pInjectorPattyPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dInjectorPattyOffsetX, 0 ,dInjectorPattyOffsetZ), "InjectorPatty", m_pInjectorPattyLogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    constexpr G4double dInjectorBun2InnerRadius = 3 * mm;
    constexpr G4double dInjectorBun2OuterRadius = dInjectorPattyOuterRadius;
    constexpr G4double dInjectorBun2HalfZ = dInjectorBun1HalfZ;
    auto *pInjectorBun2Tubs = new G4Tubs("InjectorBun2Tubs", dInjectorBun2InnerRadius, dInjectorBun2OuterRadius, dInjectorBun2HalfZ, 0, twopi);

    m_pInjectorBun2LogicalVolume = new G4LogicalVolume(pInjectorBun2Tubs, SS304L, "InjectorBun2LogicalVolume");

    constexpr G4double dInjectorBun2OffsetX = dInjectorPattyOffsetX;
    constexpr G4double dInjectorBun2OffsetZ = dInjectorPattyOffsetZ - dInjectorPattyHalfZ - dInjectorBun2HalfZ;
    m_pInjectorBun2PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dInjectorBun2OffsetX, 0, dInjectorBun2OffsetZ), "InjectorBun2", m_pInjectorBun2LogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    // define reflectivity
    // water -> the reflector
    auto *pOpReflectorSurface = new G4OpticalSurface("OpReflectorSurface");
    pOpReflectorSurface->SetType(dielectric_dielectric);
    pOpReflectorSurface->SetModel(unified);
    pOpReflectorSurface->SetFinish(groundbackpainted);
    pOpReflectorSurface->SetSigmaAlpha(0.1);
    pOpReflectorSurface->SetMaterialPropertiesTable(ePTFE->GetMaterialPropertiesTable());

    m_pBorderWater2ReflectorSide = new G4LogicalBorderSurface("ReflectorSideSurface", m_pWaterPhysicalVolume, m_pReflectorSidePhysicalVolume, pOpReflectorSurface);
    m_pBorderWater2ReflectorBottom = new G4LogicalBorderSurface("ReflectorBottomSurface", m_pWaterPhysicalVolume, m_pReflectorBottomPhysicalVolume, pOpReflectorSurface);
    m_pBorderWater2ReflectorLid = new G4LogicalBorderSurface("ReflectorLidSurface", m_pWaterPhysicalVolume, m_pReflectorLidPhysicalVolume, pOpReflectorSurface);

    // water -> the injector
    auto *pOpInjectorSurface = new G4OpticalSurface("OpInjectorSurface");
    pOpInjectorSurface->SetType(dielectric_dielectric);
    pOpInjectorSurface->SetModel(unified);
    pOpInjectorSurface->SetFinish(polishedfrontpainted);
    pOpInjectorSurface->SetMaterialPropertiesTable(SS304L->GetMaterialPropertiesTable());

    m_pBorderWater2FiberCase = new G4LogicalBorderSurface("FiberCaseSurface", m_pWaterPhysicalVolume, m_pFiberCasePhysicalVolume, pOpInjectorSurface);
    m_pBorderWater2InjectorBun1 = new G4LogicalBorderSurface("InjectorBun1Surface", m_pWaterPhysicalVolume, m_pInjectorBun1PhysicalVolume, pOpInjectorSurface);
    m_pBorderWater2InjectorBun2 = new G4LogicalBorderSurface("InjectorBun2Surface", m_pWaterPhysicalVolume, m_pInjectorBun2PhysicalVolume, pOpInjectorSurface);
    m_pBorderWater2InjectorPatty = new G4LogicalBorderSurface("InjectorPattySurface", m_pWaterPhysicalVolume, m_pInjectorPattyPhysicalVolume, pOpInjectorSurface);
}
