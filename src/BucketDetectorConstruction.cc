#include "BucketDetectorConstruction.hh"
#include "BucketMaterials.hh"
#include "BucketDetectorMessenger.hh"
#include <G4Box.hh>
#include <G4Tubs.hh>
#include <G4Polyhedra.hh>
#include <G4UnionSolid.hh>
#include <G4SubtractionSolid.hh>
#include <G4Material.hh>
#include <G4LogicalVolume.hh>
#include <G4OpticalSurface.hh>
#include <G4LogicalBorderSurface.hh>
#include <G4Transform3D.hh>
#include <G4PVPlacement.hh>
#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>
#include <G4VisAttributes.hh>
#include <vector>

BucketDetectorConstruction::BucketDetectorConstruction() : m_pMaterials(nullptr), m_pMessenger(nullptr) {
    m_pMaterials = new BucketMaterials();
    m_pMessenger = new BucketDetectorMessenger(this);
}

BucketDetectorConstruction::~BucketDetectorConstruction()
{
    delete m_pMaterials;
    delete m_pMessenger;
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
    ConstructLaserInjector(-50*mm, 0, 112*mm);

    return m_pWorldPhysicalVolume;
}

void BucketDetectorConstruction::ConstructBucket(){
    // construct volumes
    // bucket side
    constexpr G4double dBucketThickness = 2 * mm;
    constexpr G4double dBucketSideInnerRadius = 150 * mm;
    constexpr G4double dBucketSideOuterRadius = dBucketSideInnerRadius + dBucketThickness;
    constexpr G4double dBucketSideHalfZ = 150 * mm;
    auto *pBucketSideTubs = new G4Tubs("BucketSideTubs", dBucketSideInnerRadius, dBucketSideOuterRadius, dBucketSideHalfZ, 0, twopi);

    auto *SS304L = G4Material::GetMaterial("SS304L");
    m_pBucketSideLogicalVolume = new G4LogicalVolume(pBucketSideTubs, SS304L, "BucketSideLogicalVolume");
    G4Colour hBucketColor(0.7, 0.7, 0.7);
    auto* pBucketVisAtt = new G4VisAttributes(hBucketColor);
    pBucketVisAtt->SetVisibility(true);
    m_pBucketSideLogicalVolume->SetVisAttributes(pBucketVisAtt);

    m_pBucketSidePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, 0), "BucketSide", m_pBucketSideLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // bucket bottom
    constexpr G4double dBucketBottomOuterRadius = dBucketSideOuterRadius;
    constexpr G4double dBucketBottomHalfZ = 0.5 * dBucketThickness;
    auto *pBucketBottomTubs = new G4Tubs("BucketBottomTubs", 0, dBucketBottomOuterRadius, dBucketBottomHalfZ, 0, twopi);

    m_pBucketBottomLogicalVolume = new G4LogicalVolume(pBucketBottomTubs, SS304L, "BucketBottomLogicalVolume");
    m_pBucketBottomLogicalVolume->SetVisAttributes(pBucketVisAtt);

    constexpr G4double dBucketBottomOffsetZ = -dBucketSideHalfZ - dBucketBottomHalfZ;
    m_pBucketBottomPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dBucketBottomOffsetZ), "BucketBottom", m_pBucketBottomLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

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

    // bucket lid
    constexpr G4double dBucketLid1Thickness = 2 * mm;
    constexpr G4double dBucketLid1OuterRadius = dBucketSideOuterRadius;
    constexpr G4double dBucketLid1HalfZ = 0.5 * dBucketLid1Thickness;
    auto *pBucketLid1Tubs = new G4Tubs("BucketLid1Tubs", 0, dBucketLid1OuterRadius, dBucketLid1HalfZ, 0, twopi);

    constexpr G4double dBucketLid2Thickness = 3 * mm;
    constexpr G4double dBucketLid2OuterRadius = dBucketSideInnerRadius - dReflectorThickness;
    constexpr G4double dBucketLid2HalfZ = 0.5 * dBucketLid2Thickness;
    auto* pBucketLid2Tubs = new G4Tubs("BucketLid2Tubs", 0, dBucketLid2OuterRadius, dBucketLid2HalfZ, 0, twopi);

    auto* pBucketLidSolid1 = new G4UnionSolid("BucketLidSolid1", pBucketLid1Tubs, pBucketLid2Tubs, 0, G4ThreeVector(0, 0, -dBucketLid1HalfZ - dBucketLid2HalfZ));

    constexpr G4double dLaserX = -50 * mm;
    constexpr G4double dBucketLidHole1OuterRadius = 17.5 * mm;
    constexpr G4double dBucketLidThickness = dBucketLid1Thickness + dBucketLid2Thickness;
    auto* pBucketLidHole1Tubs = new G4Tubs("BucketLidHole1Tubs", 0, dBucketLidHole1OuterRadius, dBucketLidThickness, 0, twopi);

    auto* pBucketLidSolid2 = new G4SubtractionSolid("BucketLidSolid2", pBucketLidSolid1, pBucketLidHole1Tubs, 0, G4ThreeVector(dLaserX, 0, -dBucketLid2HalfZ));

    constexpr G4double dPMTX = 50 * mm;
    constexpr G4double dBucketLidHole2OuterRadius = 31.5 * mm;
    auto* pBucketLidHole2Tubs = new G4Tubs("BucketLidHole2Tubs", 0, dBucketLidHole2OuterRadius, dBucketLidThickness, 0, twopi);

    auto* pBucketLidSolid = new G4SubtractionSolid("BucketLidSolid", pBucketLidSolid2, pBucketLidHole2Tubs, 0, G4ThreeVector(dPMTX, 0, -dBucketLid2HalfZ));

    auto *Aluminium = G4Material::GetMaterial("Aluminium");
    m_pBucketLidLogicalVolume = new G4LogicalVolume(pBucketLidSolid, Aluminium, "BucketLidLogicalVolume");
    G4Colour hBucketLidColor(0.8, 0.8, 0.8);
    auto* pBucketLidVisAtt = new G4VisAttributes(hBucketLidColor);
    pBucketLidVisAtt->SetVisibility(true);
    m_pBucketLidLogicalVolume->SetVisAttributes(pBucketLidVisAtt);

    constexpr G4double dBucketLidOffsetZ = dBucketSideHalfZ + dBucketLid1HalfZ;
    m_pBucketLidPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dBucketLidOffsetZ), "BucketLid", m_pBucketLidLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // reflector lid
    constexpr G4double dReflectorLidOuterRadius = dReflectorSideInnerRadius;
    constexpr G4double dReflectorLidHalfZ = 0.5 * dReflectorThickness;
    auto *pReflectorLidTubs = new G4Tubs("ReflectorLidTubs", 0, dReflectorLidOuterRadius, dReflectorLidHalfZ, 0, twopi);

    constexpr G4double dReflectorLidHole1OuterRadius = dBucketLidHole1OuterRadius;
    auto* pReflectorLidHole1Tubs = new G4Tubs("ReflectorLidHole1Tubs", 0, dReflectorLidHole1OuterRadius, dReflectorThickness, 0, twopi);

    auto* pReflectorLidSolid1 = new G4SubtractionSolid("ReflectorLidSolid1", pReflectorLidTubs, pReflectorLidHole1Tubs, 0, G4ThreeVector(dLaserX, 0, 0));

    constexpr G4double dReflectorLidHole2OuterRadius = dBucketLidHole2OuterRadius;
    auto* pReflectorLidHole2Tubs = new G4Tubs("ReflectorLidHole2Tubs", 0, dReflectorLidHole2OuterRadius, dReflectorThickness, 0, twopi);

    auto* pReflectorLidSolid = new G4SubtractionSolid("ReflectorLidSolid", pReflectorLidSolid1, pReflectorLidHole2Tubs, 0, G4ThreeVector(dPMTX, 0, 0));

    m_pReflectorLidLogicalVolume = new G4LogicalVolume(pReflectorLidSolid, ePTFE, "ReflectorLidLogicalVolume");

    constexpr G4double dReflectorLidOffsetZ = dReflectorSideHalfZ - dBucketLid2Thickness - 0.5 * dReflectorThickness;
    m_pReflectorLidPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dReflectorLidOffsetZ), "ReflectorLid", m_pReflectorLidLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    auto* pReflectorLidForHoleTubs = new G4Tubs("ReflectorLidForHoleTubs", 0, dReflectorLidHole1OuterRadius, dReflectorLidHalfZ, 0, twopi);
    auto* pReflectorLidForHoleLogicalVolume = new G4LogicalVolume(pReflectorLidForHoleTubs, ePTFE, "ReflectorLidForHoleTubs");
    constexpr G4double dReflectorLidForHoleOffsetZ = dBucketSideHalfZ + dBucketLid1Thickness - dReflectorLidHalfZ;
    m_pReflectorLidForHolePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dLaserX, 0, dReflectorLidForHoleOffsetZ), "ReflectorLidForHole", pReflectorLidForHoleLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // water
    constexpr G4double dWater1HalfZ = 0.5 * (dReflectorSideHalfZ * 2 - dBucketLid2Thickness - dReflectorThickness * 2);
    constexpr G4double dWater1OuterRadius = dReflectorSideInnerRadius;
    auto *pWater1Tubs = new G4Tubs("Water1Tubs", 0, dWater1OuterRadius, dWater1HalfZ, 0, twopi);

    constexpr G4double dWater2HalfZ = dBucketLid1HalfZ + dBucketLid2HalfZ;
    constexpr G4double dWater2OuterRadius = dBucketLidHole1OuterRadius;
    auto *pWater2Tubs = new G4Tubs("Water2Tubs", 0, dWater2OuterRadius, dWater2HalfZ, 0, twopi);

    auto* pWaterSolid = new G4UnionSolid("WaterSolid", pWater1Tubs, pWater2Tubs, 0, G4ThreeVector(dLaserX, 0, dWater1HalfZ + dWater2HalfZ));

    auto *Water = G4Material::GetMaterial("Water");
    m_pWaterLogicalVolume = new G4LogicalVolume(pWaterSolid, Water, "WaterLogicalVolume");
    G4Colour hWaterColor(0, 0, 0.8);
    auto* pWaterVisAtt = new G4VisAttributes(hWaterColor);
    pWaterVisAtt->SetVisibility(false);
    m_pWaterLogicalVolume->SetVisAttributes(pWaterVisAtt);

    constexpr G4double dWaterOffsetZ = -dBucketLid2HalfZ;
    m_pWaterPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dWaterOffsetZ), "Water", m_pWaterLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // PMT
    // Aluminium cover including the PMT case
    constexpr G4double dPMTAluminiumCoverHalfZ = 50 * mm;
    constexpr G4double dPMTAluminiumCoverInnerRadius = 25 * mm;
    constexpr G4double dPMTAluminiumCoverOuterRadius = 31.5 * mm;
    auto* pPMTAluminiumCoverTubs = new G4Tubs("PMTAluminiumCoverTubs", dPMTAluminiumCoverInnerRadius, dPMTAluminiumCoverOuterRadius, dPMTAluminiumCoverHalfZ, 0, twopi);

    m_pPMTAluminiumCoverLogicalVolume = new G4LogicalVolume(pPMTAluminiumCoverTubs, Aluminium, "PMTAluminiumCoverLogicalVolume");
    G4Colour hPMTAluminiumCoverColor(0.8, 0.8, 0.8);
    auto* pPMTAluminiumVisAtt = new G4VisAttributes(hPMTAluminiumCoverColor);
    pPMTAluminiumVisAtt->SetVisibility(true);
    m_pPMTAluminiumCoverLogicalVolume->SetVisAttributes(pPMTAluminiumVisAtt);

    constexpr G4double dPMTOffsetX = dPMTX;
    constexpr G4double dPMTOffsetZ = dWaterOffsetZ + dWater1HalfZ + dPMTAluminiumCoverHalfZ;
    m_pPMTAluminiumCoverPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dPMTOffsetX, 0, dPMTOffsetZ), "PMTAluminiumCover", m_pPMTAluminiumCoverLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // Aluminium lid
    constexpr G4double dPMTAluminiumLidHalfZ = 1 * mm;
    constexpr G4double dPMTAluminiumLidOuterRadius = dPMTAluminiumCoverInnerRadius;
    auto* pPMTAluminiumLidTubs = new G4Tubs("PMTAluminiumLidTubs", 0, dPMTAluminiumLidOuterRadius, dPMTAluminiumLidHalfZ, 0, twopi);

    m_pPMTAluminiumLidLogicalVolume = new G4LogicalVolume(pPMTAluminiumLidTubs, Aluminium, "PMTAluminiumLidLogicalVolume");
    m_pPMTAluminiumLidLogicalVolume->SetVisAttributes(pPMTAluminiumVisAtt);

    constexpr G4double dPMTAluminiumLidOffsetZ = dPMTOffsetZ + dPMTAluminiumCoverHalfZ - dPMTAluminiumLidHalfZ;
    m_pPMTAluminiumLidPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dPMTOffsetX, 0, dPMTAluminiumLidOffsetZ), "PMTAluminiumLid", m_pPMTAluminiumLidLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // Window
    constexpr G4double dPMTWindowHalfZ = 0.5 * mm;
    constexpr G4double dPMTWindowOuterRadius = dPMTAluminiumCoverInnerRadius;
    auto* pPMTWindowTubs = new G4Tubs("PMTWindowTubs", 0, dPMTWindowOuterRadius, dPMTWindowHalfZ, 0, twopi);

    auto* Glass = G4Material::GetMaterial("Glass");
    m_pPMTWindowLogicalVolume = new G4LogicalVolume(pPMTWindowTubs, Glass, "PMTWindowLogicalVolume");
    G4Colour hPMTWindowColor(1, 0.75, 0);
    auto* pPMTWindowVisAtt = new G4VisAttributes(hPMTWindowColor);
    pPMTWindowVisAtt->SetVisibility(true);
    m_pPMTWindowLogicalVolume->SetVisAttributes(pPMTWindowVisAtt);

    constexpr G4double dPMTWindowOffsetZ = dWaterOffsetZ + dWater1HalfZ + dPMTWindowHalfZ;
    m_pPMTWindowPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dPMTOffsetX, 0, dPMTWindowOffsetZ), "PMTWindow", m_pPMTWindowLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // Photocathode
    // its name is just "PMT"
    constexpr G4double dPMTPhotocathodeHalfZ = 1 * mm;
    constexpr G4double dPMTPhotocathodeOuterRadius = dPMTWindowOuterRadius;
    auto* pPMTPhotocathodeTubs = new G4Tubs("PMTPhotocathodeTubs", 0, dPMTPhotocathodeOuterRadius, dPMTPhotocathodeHalfZ, 0, twopi);

    auto* Bialkali = G4Material::GetMaterial("Air"); // FIXME define Bialkali
    m_pPMTPhotocathodeLogicalVolume = new G4LogicalVolume(pPMTPhotocathodeTubs, Bialkali, "PMTPhotocathodeLogicalVolume");
    G4Colour hPMTWPhotocathodeColor(1, 0, 0);
    auto* pPMTPhotocathodeVisAtt = new G4VisAttributes(hPMTWPhotocathodeColor);
    pPMTPhotocathodeVisAtt->SetVisibility(true);
    m_pPMTPhotocathodeLogicalVolume->SetVisAttributes(pPMTPhotocathodeVisAtt);

    constexpr G4double dPMTPhotocathodeOffsetZ = dPMTWindowOffsetZ + dPMTWindowHalfZ + dPMTPhotocathodeHalfZ;
    m_pPMTPhotocathodePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(dPMTOffsetX, 0, dPMTPhotocathodeOffsetZ), "PMT", m_pPMTPhotocathodeLogicalVolume, m_pWorldPhysicalVolume, false, 0, true);

    // define reflectivity
    // water -> the reflector
    auto *pOpReflectorSurface = new G4OpticalSurface("OpReflectorSurface");
    pOpReflectorSurface->SetType(dielectric_dielectric);
    pOpReflectorSurface->SetModel(unified);
    pOpReflectorSurface->SetFinish(ground);
    pOpReflectorSurface->SetSigmaAlpha(m_pMaterials->ePTFESigmaAlpha());
    pOpReflectorSurface->SetMaterialPropertiesTable(ePTFE->GetMaterialPropertiesTable());

    m_pBorderWater2ReflectorSide = new G4LogicalBorderSurface("ReflectorSideSurface", m_pWaterPhysicalVolume, m_pReflectorSidePhysicalVolume, pOpReflectorSurface);
    m_pBorderWater2ReflectorBottom = new G4LogicalBorderSurface("ReflectorBottomSurface", m_pWaterPhysicalVolume, m_pReflectorBottomPhysicalVolume, pOpReflectorSurface);
    m_pBorderWater2ReflectorLid = new G4LogicalBorderSurface("ReflectorLidSurface", m_pWaterPhysicalVolume, m_pReflectorLidPhysicalVolume, pOpReflectorSurface);
    m_pBorderWater2ReflectorLidForHole = new G4LogicalBorderSurface("ReflectorLidForHoleSurface", m_pWaterPhysicalVolume, m_pReflectorLidForHolePhysicalVolume, pOpReflectorSurface);

    // water -> the PMT aluminium cover
    auto *pOpPMTAluminiumCoverSurface = new G4OpticalSurface("OpPMTAluminiumCoverSurface");
    pOpPMTAluminiumCoverSurface->SetType(dielectric_metal);
    pOpPMTAluminiumCoverSurface->SetModel(unified);
    pOpPMTAluminiumCoverSurface->SetFinish(polished);
    pOpPMTAluminiumCoverSurface->SetMaterialPropertiesTable(Aluminium->GetMaterialPropertiesTable());

    m_pBorderWater2PMTAluminiumCover= new G4LogicalBorderSurface("AluminiumCoverSurface", m_pWaterPhysicalVolume, m_pPMTAluminiumCoverPhysicalVolume, pOpPMTAluminiumCoverSurface);
}

void BucketDetectorConstruction::ConstructLaserInjector(G4double dLaserIrradiationX, G4double dLaserIrradiationY, G4double dLaserIrradiationZ){
    // Construct the laser injector head and the steel fiber case
    G4ThreeVector hLaserIradiationPoint(dLaserIrradiationX, dLaserIrradiationY, dLaserIrradiationZ);

    // injector head
    // front part
    constexpr G4double dInjectorHeadOuterRadius = 17 * mm;
    constexpr G4double dInjectorHeadInnerRadius = 3 * mm;
    constexpr G4double dInjectorHeadFrontHalfZ = 3.5 * mm;
    auto* pInjectorHeadFrontTubs = new G4Tubs("InjectorHeadFrontTubs", dInjectorHeadInnerRadius, dInjectorHeadOuterRadius, dInjectorHeadFrontHalfZ, 0, twopi);

    auto* SS304L = G4Material::GetMaterial("SS304L");
    m_pInjectorHeadFrontLogicalVolume = new G4LogicalVolume(pInjectorHeadFrontTubs, SS304L, "InjectorHeadFrontLogicalVolume");
    G4Colour hInjectorColor(0, 0.75, 1);
    auto* pInjectorHeadVisAtt = new G4VisAttributes(hInjectorColor);
    pInjectorHeadVisAtt->SetVisibility(true);
    m_pInjectorHeadFrontLogicalVolume->SetVisAttributes(pInjectorHeadVisAtt);

    constexpr G4double dInjectorHeadFrontOffsetZ = -dInjectorHeadFrontHalfZ;
    m_pInjectorHeadFrontPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dInjectorHeadFrontOffsetZ) + hLaserIradiationPoint, "InjectorHeadFront", m_pInjectorHeadFrontLogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    // middle part
    constexpr G4double dInjectorHeadMiddleHalfZ = 1.5 * mm;
    auto* pInjectorHeadMiddleTubs = new G4Tubs("InjectorHeadMiddleTubTubs", 0, dInjectorHeadOuterRadius, dInjectorHeadMiddleHalfZ, 0, twopi);

    m_pInjectorHeadMiddleLogicalVolume = new G4LogicalVolume(pInjectorHeadMiddleTubs, SS304L, "InjectorHeadMiddleLogicalVolume");
    G4Colour hInjectorHeadMiddleColor(0, 0, 1);
    auto* pInjectorHeadMiddleVisAtt = new G4VisAttributes(hInjectorHeadMiddleColor);
    pInjectorHeadMiddleVisAtt->SetVisibility(true);
    m_pInjectorHeadMiddleLogicalVolume->SetVisAttributes(pInjectorHeadMiddleVisAtt);

    constexpr G4double dInjectorHeadMiddleOffsetZ = dInjectorHeadMiddleHalfZ;
    m_pInjectorHeadMiddlePhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dInjectorHeadMiddleOffsetZ) + hLaserIradiationPoint, "InjectorHeadMiddle", m_pInjectorHeadMiddleLogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    // bottom part
    constexpr G4double dInjectorHeadBackHalfZ = 3.5 * mm;
    auto* pInjectorHeadBackTubs = new G4Tubs("InjectorHeadBackTubTubs", 0, dInjectorHeadOuterRadius, dInjectorHeadBackHalfZ, 0, twopi);

    m_pInjectorHeadBackLogicalVolume = new G4LogicalVolume(pInjectorHeadBackTubs, SS304L, "InjectorHeadBackLogicalVolume");
    m_pInjectorHeadBackLogicalVolume->SetVisAttributes(pInjectorHeadVisAtt);

    constexpr G4double dInjectorHeadBackOffsetZ = 2 * dInjectorHeadMiddleHalfZ + dInjectorHeadBackHalfZ;
    m_pInjectorHeadBackPhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dInjectorHeadBackOffsetZ) + hLaserIradiationPoint, "InjectorHeadBack", m_pInjectorHeadBackLogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    // fiber case
    // tube part 1
    constexpr G4double dInjectorFiberCase1OuterRadius = 3 * mm;
    constexpr G4double dInjectorFiberCase1HalfZ = 5.5 * mm;
    auto* pInjectorFiberCase1Tubs = new G4Tubs("InjectorFiberCase1Tubs", 0, dInjectorFiberCase1OuterRadius, dInjectorFiberCase1HalfZ, 0, twopi);

    m_pInjectorFiberCase1LogicalVolume = new G4LogicalVolume(pInjectorFiberCase1Tubs, SS304L, "InjectorFiberCase1LogicalVolume");
    auto* pInjectorFiberCaseVisAtt = new G4VisAttributes(hInjectorColor);
    pInjectorFiberCaseVisAtt->SetVisibility(true);
    m_pInjectorFiberCase1LogicalVolume->SetVisAttributes(pInjectorFiberCaseVisAtt);

    constexpr G4double dInjectorFiberCase1OffsetZ = 2 * (dInjectorHeadMiddleHalfZ + dInjectorHeadBackHalfZ) + dInjectorFiberCase1HalfZ;;
    m_pInjectorFiberCase1PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dInjectorFiberCase1OffsetZ) + hLaserIradiationPoint, "InjectorFiberCase1", m_pInjectorFiberCase1LogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    // nut 1
    constexpr G4double dInjectorFiberCaseNut1HalfZ = 2.5 * mm;
    constexpr G4double dInjectorFiberCaseNut1OuterRadius = 5.5 * mm;
    constexpr G4int iInjectorFiberCaseNut1NumZPlanes = 2;
    constexpr G4int iInjectorFiberCaseNut1NumSide = 6;
    constexpr G4double dInjectorFiberCaseNut1ZPlane[iInjectorFiberCaseNut1NumZPlanes] = {dInjectorFiberCaseNut1HalfZ, -dInjectorFiberCaseNut1HalfZ};
    constexpr G4double dInjectorFiberCaseNut1RInner[iInjectorFiberCaseNut1NumSide] = {0, 0, 0, 0, 0, 0};
    constexpr G4double dInjectorFiberCaseNut1ROuter[iInjectorFiberCaseNut1NumSide] = {dInjectorFiberCaseNut1OuterRadius, dInjectorFiberCaseNut1OuterRadius, dInjectorFiberCaseNut1OuterRadius, dInjectorFiberCaseNut1OuterRadius, dInjectorFiberCaseNut1OuterRadius, dInjectorFiberCaseNut1OuterRadius};
    auto* pInjectorFiberCaseNut1Polyhedra = new G4Polyhedra("InjectorFiberCaseNut1Polyhedra", 0, twopi, iInjectorFiberCaseNut1NumSide, iInjectorFiberCaseNut1NumZPlanes,
            dInjectorFiberCaseNut1ZPlane, dInjectorFiberCaseNut1RInner, dInjectorFiberCaseNut1ROuter);

    m_pInjectorFiberCaseNut1LogicalVolume = new G4LogicalVolume(pInjectorFiberCaseNut1Polyhedra, SS304L, "InjectorFiberCaseNut1LogicalVolume");
    m_pInjectorFiberCaseNut1LogicalVolume->SetVisAttributes(pInjectorFiberCaseVisAtt);

    constexpr G4double dInjectorFiberCaseNut1OffsetZ = 2 * (dInjectorHeadMiddleHalfZ + dInjectorHeadBackHalfZ + dInjectorFiberCase1HalfZ) + dInjectorFiberCaseNut1HalfZ;
    m_pInjectorFiberCaseNut1PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dInjectorFiberCaseNut1OffsetZ) + hLaserIradiationPoint, "InjectorFiberCaseNut1", m_pInjectorFiberCaseNut1LogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    // tube 2
    constexpr G4double dInjectorFiberCase2OuterRadius = dInjectorFiberCase1OuterRadius;
    constexpr G4double dInjectorFiberCase2HalfZ = 1.5 * mm;
    auto* pInjectorFiberCase2Tubs = new G4Tubs("InjectorFiberCase2Tubs", 0, dInjectorFiberCase2OuterRadius, dInjectorFiberCase2HalfZ, 0, twopi);

    m_pInjectorFiberCase2LogicalVolume = new G4LogicalVolume(pInjectorFiberCase2Tubs, SS304L, "InjectorFiberCase2LogicalVolume");
    m_pInjectorFiberCase2LogicalVolume->SetVisAttributes(pInjectorFiberCaseVisAtt);

    constexpr G4double dInjectorFiberCase2OffsetZ = dInjectorFiberCaseNut1OffsetZ + dInjectorFiberCaseNut1HalfZ + dInjectorFiberCase2HalfZ;;
    m_pInjectorFiberCase2PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dInjectorFiberCase2OffsetZ) + hLaserIradiationPoint, "InjectorFiberCase2", m_pInjectorFiberCase2LogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    // nut 2
    constexpr G4double dInjectorFiberCaseNut2HalfZ = 4 * mm;
    constexpr G4double dInjectorFiberCaseNut2OuterRadius = dInjectorFiberCaseNut1OuterRadius;
    constexpr G4int iInjectorFiberCaseNut2NumZPlanes = 2;
    constexpr G4int iInjectorFiberCaseNut2NumSide = 6;
    constexpr G4double dInjectorFiberCaseNut2ZPlane[iInjectorFiberCaseNut2NumZPlanes] = {dInjectorFiberCaseNut2HalfZ, -dInjectorFiberCaseNut2HalfZ};
    constexpr G4double dInjectorFiberCaseNut2RInner[iInjectorFiberCaseNut2NumSide] = {0, 0, 0, 0, 0, 0};
    constexpr G4double dInjectorFiberCaseNut2ROuter[iInjectorFiberCaseNut2NumSide] = {dInjectorFiberCaseNut2OuterRadius, dInjectorFiberCaseNut2OuterRadius, dInjectorFiberCaseNut2OuterRadius, dInjectorFiberCaseNut2OuterRadius, dInjectorFiberCaseNut2OuterRadius, dInjectorFiberCaseNut2OuterRadius};
    auto* pInjectorFiberCaseNut2Polyhedra = new G4Polyhedra("InjectorFiberCaseNut2Polyhedra", 0, twopi, iInjectorFiberCaseNut2NumSide, iInjectorFiberCaseNut2NumZPlanes,
            dInjectorFiberCaseNut2ZPlane, dInjectorFiberCaseNut2RInner, dInjectorFiberCaseNut2ROuter);

    m_pInjectorFiberCaseNut2LogicalVolume = new G4LogicalVolume(pInjectorFiberCaseNut2Polyhedra, SS304L, "InjectorFiberCaseNut2LogicalVolume");
    m_pInjectorFiberCaseNut2LogicalVolume->SetVisAttributes(pInjectorFiberCaseVisAtt);

    constexpr G4double dInjectorFiberCaseNut2OffsetZ = dInjectorFiberCase2OffsetZ + dInjectorFiberCase2HalfZ + dInjectorFiberCaseNut2HalfZ;
    m_pInjectorFiberCaseNut2PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dInjectorFiberCaseNut2OffsetZ) + hLaserIradiationPoint, "InjectorFiberCaseNut2", m_pInjectorFiberCaseNut2LogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    // nut 3
    constexpr G4double dInjectorFiberCaseNut3OuterRadius = 5.5 * mm;
    constexpr G4double dInjectorFiberCaseNut3HalfZ = 1.5 * mm;
    auto* pInjectorFiberCaseNut3Tubs = new G4Tubs("InjectorFiberCaseNut3Tubs", 0, dInjectorFiberCaseNut3OuterRadius, dInjectorFiberCaseNut3HalfZ, 0, twopi);

    m_pInjectorFiberCaseNut3LogicalVolume = new G4LogicalVolume(pInjectorFiberCaseNut3Tubs, SS304L, "InjectorFiberCaseNut3LogicalVolume");
    m_pInjectorFiberCaseNut3LogicalVolume->SetVisAttributes(pInjectorFiberCaseVisAtt);

    constexpr G4double dInjectorFiberCaseNut3OffsetZ = dInjectorFiberCaseNut2OffsetZ + dInjectorFiberCaseNut2HalfZ + dInjectorFiberCaseNut3HalfZ;
    m_pInjectorFiberCaseNut3PhysicalVolume = new G4PVPlacement(0, G4ThreeVector(0, 0, dInjectorFiberCaseNut3OffsetZ) + hLaserIradiationPoint, "InjectorFiberCaseNut3", m_pInjectorFiberCaseNut3LogicalVolume, m_pWaterPhysicalVolume, false, 0, true);

    // fiber head nut
    // front part
    constexpr G4double dInjectorHeadNut1HalfZ = 2.5 * mm;
    constexpr G4double dInjectorHeadNut1OuterRadius = 3.5 * mm;
    auto* pInjectorHeadNut1Tubs = new G4Tubs("InjectorHeadTubs", 0, dInjectorHeadNut1OuterRadius, dInjectorHeadNut1HalfZ, 0, twopi);

    constexpr G4double dInjectorHeadNut1InnerHalfZ = 1.0 * mm;
    constexpr G4double dInjectorHeadNut1InnerRadius = 1.5 * mm;
    constexpr G4int iInjectorHeadNut1NumZPlanes = 2;
    constexpr G4int iInjectorHeadNut1NumSide = 6;
    constexpr G4double dInjectorHeadNut1ZPlane[iInjectorHeadNut1NumZPlanes] = {dInjectorHeadNut1InnerHalfZ, -dInjectorHeadNut1InnerHalfZ};
    constexpr G4double dInjectorHeadNut1RInner[iInjectorHeadNut1NumSide] = {0, 0, 0, 0, 0, 0};
    constexpr G4double dInjectorHeadNut1ROuter[iInjectorHeadNut1NumSide] = {dInjectorHeadNut1InnerRadius, dInjectorHeadNut1InnerRadius, dInjectorHeadNut1InnerRadius, dInjectorHeadNut1InnerRadius, dInjectorHeadNut1InnerRadius, dInjectorHeadNut1InnerRadius};
    auto* pInjectorHeadNut1Polyhedra = new G4Polyhedra("InjectorHeadNut1Polyhedra", 0, twopi, iInjectorHeadNut1NumSide, iInjectorHeadNut1NumZPlanes,
            dInjectorHeadNut1ZPlane, dInjectorHeadNut1RInner, dInjectorHeadNut1ROuter);

    auto* pInjectorHeadNut1Solid = new G4SubtractionSolid("InjectorHeadNut1Solid", pInjectorHeadNut1Tubs, pInjectorHeadNut1Polyhedra, 0, G4ThreeVector(0, 0, -dInjectorHeadNut1HalfZ + dInjectorHeadNut1InnerHalfZ));

    m_pInjectorHeadNut1LogicalVolume = new G4LogicalVolume(pInjectorHeadNut1Solid, SS304L, "InjectorHeadNut1LogicalVolume");
    m_pInjectorHeadNut1LogicalVolume->SetVisAttributes(pInjectorHeadVisAtt);

    constexpr G4double dInjectorHeadNut1OffsetZ = dInjectorHeadFrontOffsetZ - dInjectorHeadFrontHalfZ - dInjectorHeadNut1HalfZ;
    G4ThreeVector hInjectorHeadNut1Position(dInjectorHeadOuterRadius - dInjectorHeadNut1OuterRadius, 0, dInjectorHeadNut1OffsetZ);
    for (int i = 0; i < 6; ++i) {
        m_pInjectorHeadNut1PhysicalVolume[i] = new G4PVPlacement(0, hInjectorHeadNut1Position.rotateZ(60*deg) + hLaserIradiationPoint, "InjectorHeadNut1" + std::to_string(i), m_pInjectorHeadNut1LogicalVolume, m_pWaterPhysicalVolume, false, 0, true);
    }

    // back part
    constexpr G4double dInjectorHeadNut2HalfZ = 2 * mm;
    constexpr G4double dInjectorHeadNut2OuterRadius = dInjectorHeadNut1OuterRadius;
    constexpr G4int iInjectorHeadNut2NumZPlanes = 2;
    constexpr G4int iInjectorHeadNut2NumSide = 6;
    constexpr G4double dInjectorHeadNut2ZPlane[iInjectorHeadNut2NumZPlanes] = {dInjectorHeadNut2HalfZ, -dInjectorHeadNut2HalfZ};
    constexpr G4double dInjectorHeadNut2RInner[iInjectorHeadNut2NumSide] = {0, 0, 0, 0, 0, 0};
    constexpr G4double dInjectorHeadNut2ROuter[iInjectorHeadNut2NumSide] = {dInjectorHeadNut2OuterRadius, dInjectorHeadNut2OuterRadius, dInjectorHeadNut2OuterRadius, dInjectorHeadNut2OuterRadius, dInjectorHeadNut2OuterRadius, dInjectorHeadNut2OuterRadius};
    auto* pInjectorHeadNut2Polyhedra = new G4Polyhedra("InjectorHeadNut2Polyhedra", 0, twopi, iInjectorHeadNut2NumSide, iInjectorHeadNut2NumZPlanes,
            dInjectorHeadNut2ZPlane, dInjectorHeadNut2RInner, dInjectorHeadNut2ROuter);

    m_pInjectorHeadNut2LogicalVolume = new G4LogicalVolume(pInjectorHeadNut2Polyhedra, SS304L, "jectorHeadNut2LogicalVolume");
    m_pInjectorHeadNut2LogicalVolume->SetVisAttributes(pInjectorHeadVisAtt);

    constexpr G4double dInjectorHeadNut2OffsetZ = dInjectorHeadBackOffsetZ + dInjectorHeadBackHalfZ + dInjectorHeadNut2HalfZ;
    G4ThreeVector hInjectorHeadNut2Position(dInjectorHeadOuterRadius - dInjectorHeadNut2OuterRadius * 2 / sqrt(3), 0, dInjectorHeadNut2OffsetZ);
    for (int i = 0; i < 6; ++i) {
        m_pInjectorHeadNut2PhysicalVolume[i] = new G4PVPlacement(0, hInjectorHeadNut2Position.rotateZ(60*deg) + hLaserIradiationPoint, "InjectorHeadNut2" + std::to_string(i), m_pInjectorHeadNut2LogicalVolume, m_pWaterPhysicalVolume, false, 0, true);
    }

    // back part 2
    // protruding screw
    constexpr G4double dInjectorHeadNut3HalfZ = 1.5 * mm;
    constexpr G4double dInjectorHeadNut3OuterRadius = 1.5 * mm;
    auto* pInjectorHeadNut3Tubs = new G4Tubs("InjectorHeadTubs", 0, dInjectorHeadNut3OuterRadius, dInjectorHeadNut3HalfZ, 0, twopi);

    m_pInjectorHeadNut3LogicalVolume = new G4LogicalVolume(pInjectorHeadNut3Tubs, SS304L, "InjectorHeadNut3LogicalVolume");
    m_pInjectorHeadNut3LogicalVolume->SetVisAttributes(pInjectorHeadVisAtt);

    constexpr G4double dInjectorHeadNut3OffsetZ = dInjectorHeadNut2OffsetZ + dInjectorHeadNut2HalfZ + dInjectorHeadNut3HalfZ;
    G4ThreeVector hInjectorHeadNut3Position(dInjectorHeadOuterRadius - dInjectorHeadNut2OuterRadius * 2 / sqrt(3), 0, dInjectorHeadNut3OffsetZ);
    for (int i = 0; i < 6; ++i) {
        m_pInjectorHeadNut3PhysicalVolume[i] = new G4PVPlacement(0, hInjectorHeadNut3Position.rotateZ(60*deg) + hLaserIradiationPoint, "InjectorHeadNut3" + std::to_string(i), m_pInjectorHeadNut3LogicalVolume, m_pWaterPhysicalVolume, false, 0, true);
    }

    // water -> the laser injector
    auto *pOpLaserInjectorSurface = new G4OpticalSurface("OpLaserInjectorSurface");
    pOpLaserInjectorSurface->SetType(dielectric_metal);
    pOpLaserInjectorSurface->SetModel(unified);
    pOpLaserInjectorSurface->SetFinish(polished);
    pOpLaserInjectorSurface->SetMaterialPropertiesTable(SS304L->GetMaterialPropertiesTable());

    m_pBorderWater2InjectorHeadFront = new G4LogicalBorderSurface("InjectorHeadFrontSurface", m_pWaterPhysicalVolume, m_pInjectorHeadFrontPhysicalVolume, pOpLaserInjectorSurface);
    m_pBorderWater2InjectorHeadMiddle = new G4LogicalBorderSurface("InjectorHeadMiddleSurface", m_pWaterPhysicalVolume, m_pInjectorHeadMiddlePhysicalVolume, pOpLaserInjectorSurface);
    m_pBorderWater2InjectorHeadBack = new G4LogicalBorderSurface("InjectorHeadBackSurface", m_pWaterPhysicalVolume, m_pInjectorHeadBackPhysicalVolume, pOpLaserInjectorSurface);
    m_pBorderWater2InjectorFiberCase1 = new G4LogicalBorderSurface("InjectorFiberCase1Surface", m_pWaterPhysicalVolume, m_pInjectorFiberCase1PhysicalVolume, pOpLaserInjectorSurface);
    m_pBorderWater2InjectorFiberCaseNut1 = new G4LogicalBorderSurface("InjectorFiberCaseNut1Surface", m_pWaterPhysicalVolume, m_pInjectorFiberCaseNut1PhysicalVolume, pOpLaserInjectorSurface);
    m_pBorderWater2InjectorFiberCase2 = new G4LogicalBorderSurface("InjectorFiberCase2Surface", m_pWaterPhysicalVolume, m_pInjectorFiberCase1PhysicalVolume, pOpLaserInjectorSurface);
    m_pBorderWater2InjectorFiberCaseNut2 = new G4LogicalBorderSurface("InjectorFiberCaseNut2Surface", m_pWaterPhysicalVolume, m_pInjectorFiberCaseNut2PhysicalVolume, pOpLaserInjectorSurface);
    m_pBorderWater2InjectorFiberCaseNut3 = new G4LogicalBorderSurface("InjectorFiberCaseNut3Surface", m_pWaterPhysicalVolume, m_pInjectorFiberCaseNut3PhysicalVolume, pOpLaserInjectorSurface);
    for (int i = 0; i < 6; ++i) {
        m_pBorderWater2InjectorHeadNut1[i] = new G4LogicalBorderSurface("InjectorHeadNut1Surface" + std::to_string(i), m_pWaterPhysicalVolume, m_pInjectorHeadNut1PhysicalVolume[i], pOpLaserInjectorSurface);
        m_pBorderWater2InjectorHeadNut2[i] = new G4LogicalBorderSurface("InjectorHeadNut2Surface" + std::to_string(i), m_pWaterPhysicalVolume, m_pInjectorHeadNut2PhysicalVolume[i], pOpLaserInjectorSurface);
        m_pBorderWater2InjectorHeadNut3[i] = new G4LogicalBorderSurface("InjectorHeadNut3Surface" + std::to_string(i), m_pWaterPhysicalVolume, m_pInjectorHeadNut3PhysicalVolume[i], pOpLaserInjectorSurface);
    }
}
