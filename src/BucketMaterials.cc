#include "BucketMaterials.hh"
#include <G4Material.hh>
#include <G4NistManager.hh>
#include <G4SystemOfUnits.hh>

BucketMaterials::BucketMaterials(){}

BucketMaterials::~BucketMaterials(){}

void BucketMaterials::DefineMaterials() const {
    G4NistManager* pNistManager = G4NistManager::Instance();

    // Air
    pNistManager->FindOrBuildMaterial("G4_AIR");
    auto *Air = new G4Material("Air", G4Material::GetMaterial("G4_AIR")->GetDensity(), 1);
    Air->AddMaterial(G4Material::GetMaterial("G4_AIR"), 100.0 * perCent);

    // Water
    pNistManager->FindOrBuildMaterial("G4_WATER");
    auto *Water = new G4Material("Water", G4Material::GetMaterial("G4_WATER")->GetDensity(), 1);
    Water->AddMaterial(G4Material::GetMaterial("G4_WATER"), 100.0 * perCent);

    constexpr unsigned int nWater = 2;
    G4double dWaterEnergy[nWater] = {1 * eV, 5 * eV};
    G4double dWaterRefractiveIndex[nWater] = {1.33, 1.33};
    G4double dWaterAbsorptionLength[nWater] = {m_dWaterAbslength, m_dWaterAbslength};
    G4double dWaterRayleighScatterLength[nWater] = {400 * m, 400 * m};

    auto *pWaterMPT = new G4MaterialPropertiesTable();
    pWaterMPT->AddProperty("RINDEX", dWaterEnergy, dWaterRefractiveIndex, nWater);
    pWaterMPT->AddProperty("ABSLENGTH", dWaterEnergy, dWaterAbsorptionLength, nWater);
    pWaterMPT->AddProperty("RAYLEIGH", dWaterEnergy, dWaterRayleighScatterLength, nWater);
    G4cout << "--------------------> Water MPT" << G4endl;
    pWaterMPT->DumpTable();

    Water->SetMaterialPropertiesTable(pWaterMPT);

    // ePTFE
    auto *ePTFE = new G4Material("ePTFE", 0.7 * g / cm3, 2, kStateSolid);
    auto *C = new G4Element("C", "C", 6., 12.011 * g / mole);
    auto *F = new G4Element("F", "F", 9., 18.998 * g / mole);
    ePTFE->AddElement(C, 0.240183);
    ePTFE->AddElement(F, 0.759817);

    constexpr G4int nEPTFE = 2;
    G4double dEPTFEEnergy[nEPTFE] = {1 * eV, 5 * eV};
    G4double dEPTFEReflectivity[nEPTFE] = {m_dePTFEReflectivity, m_dePTFEReflectivity};
    G4double dEPTFERefractiveIndex[nEPTFE] = {1.33, 1.33};
    G4double dEPTFESpecularLobe[nEPTFE] = {m_dePTFESpecularLobeConstant, m_dePTFESpecularLobeConstant};
    G4double dEPTFESpecularSpike[nEPTFE] = {m_dePTFESpecularSpikeConstant, m_dePTFESpecularSpikeConstant};
    G4double dEPTFEBackscatter[nEPTFE] = {m_dePTFEBackscatteringConstant, m_dePTFEBackscatteringConstant};
    G4double dEPTFEEfficiency[nEPTFE] = {1.0, 1.0};
    G4double dEPTFEAbsorptionLength[nEPTFE] = {1.0 * mm, 1.0 * mm};
    G4double dEPTFESurfaceTransmittance[nEPTFE] = {1e-20, 1e-20};

    G4MaterialPropertiesTable *pEPTFEMPT = new G4MaterialPropertiesTable();

    pEPTFEMPT->AddProperty("REFLECTIVITY", dEPTFEEnergy, dEPTFEReflectivity, nEPTFE);
    pEPTFEMPT->AddProperty("RINDEX", dEPTFEEnergy, dEPTFERefractiveIndex, nEPTFE);
    pEPTFEMPT->AddProperty("ABSLENGTH", dEPTFEEnergy, dEPTFEAbsorptionLength, nEPTFE);
    pEPTFEMPT->AddProperty("SPECULARLOBECONSTANT", dEPTFEEnergy, dEPTFESpecularLobe, nEPTFE);
    pEPTFEMPT->AddProperty("SPECULARSPIKECONSTANT", dEPTFEEnergy, dEPTFESpecularSpike, nEPTFE);
    pEPTFEMPT->AddProperty("BACKSCATTERCONSTANT", dEPTFEEnergy, dEPTFEBackscatter, nEPTFE);
    pEPTFEMPT->AddProperty("EFFICIENCY", dEPTFEEnergy, dEPTFEEfficiency, nEPTFE);
    pEPTFEMPT->AddProperty("TRANSMITTANCE", dEPTFEEnergy, dEPTFESurfaceTransmittance, nEPTFE);
    G4cout << "--------------------> ePTFE MPT" << G4endl;
    pEPTFEMPT->DumpTable();

    ePTFE->SetMaterialPropertiesTable(pEPTFEMPT);

    // Aluminium
    auto *Aluminium = new G4Material("Aluminium", 2.7* g / cm3, 1, kStateSolid);
    auto *Al = new G4Element("Al", "Al", 13, 26.982 * g / mole);
    Aluminium->AddElement(Al, 1);

    constexpr G4int nAluminium = 2;
    G4double dAluminiumEnergy[nAluminium] = {1 * eV, 5 * eV};
    G4double dAluminiumReflectivity[nAluminium] = {0.9, 0.9};

    auto* pAluminiumMPT = new G4MaterialPropertiesTable();

    pAluminiumMPT->AddProperty("REFLECTIVITY", dAluminiumEnergy, dAluminiumReflectivity, nAluminium);
    G4cout << "--------------------> Aluminium MPT" << G4endl;
    pAluminiumMPT->DumpTable();
    Aluminium->SetMaterialPropertiesTable(pAluminiumMPT);

    // SS304L
    auto *SS304L = new G4Material("SS304L", 8.00 * g / cm3, 5, kStateSolid);
    auto *Fe = new G4Element("Fe", "Fe", 26, 55.850 * g / mole);
    auto *Cr = new G4Element("Cr", "Cr", 24, 51.996 * g / mole);
    auto *Ni = new G4Element("Ni", "Ni", 28, 58.693 * g / mole);
    auto *Mn = new G4Element("Mn", "Mn", 25, 54.938 * g / mole);
    auto *Si = new G4Element("Si", "Si", 14, 28.086 * g / mole);
    SS304L->AddElement(Fe, 0.65);
    SS304L->AddElement(Cr, 0.20);
    SS304L->AddElement(Ni, 0.12);
    SS304L->AddElement(Mn, 0.02);
    SS304L->AddElement(Si, 0.01);

    constexpr G4int nSS304L = 2;
    G4double dSS304LEnergy[nSS304L] = {1 * eV, 5 * eV};
    G4double dSS304LReflectivity[nSS304L] = {m_dSteelReflectivity, m_dSteelReflectivity};

    auto *pSS304LMPT = new G4MaterialPropertiesTable();
    pSS304LMPT->AddProperty("REFLECTIVITY", dSS304LEnergy, dSS304LReflectivity, nSS304L);
    G4cout << "--------------------> SS304L MPT" << G4endl;
    pSS304LMPT->DumpTable();

    SS304L->SetMaterialPropertiesTable(pSS304LMPT);

    // Glass
    auto *Glass = new G4Material("Glass", 2.201 * g / cm3 , 2, kStateSolid);
    auto *O = new G4Element("O", "O", 8, 15.999 * g / mole);
    Glass->AddElement(Si, 1);
    Glass->AddElement(O, 2);

    // Optical properties Glass
    constexpr G4int nGlass = 7;
    G4double dGlassEnergy[nGlass] = {1.55*eV, 2.0664*eV, 2.48*eV, 2.755*eV, 3.1*eV, 4.133*eV, 6.2*eV};
    // Cauchy dispersion law n = 1.472 + 3760 / wavelength**2
    G4double dGlassRefractiveIndex[nGlass] = {1.478, 1.482, 1.487, 1.491, 1.496, 1.51, 1.57};
    G4double dGlassAbsorbtionLength[nGlass] = {300*mm, 300*mm, 300*mm, 300*mm, 300*mm, 300*mm, 300*mm};
    G4double dGlassReflectivity[nGlass] = {1, 1, 1, 1, 1, 1, 1};
    G4double dGlassTransmittance[nGlass] = {1 - DBL_MIN, 1 - DBL_MIN, 1 - DBL_MIN, 1 - DBL_MIN, 1 - DBL_MIN, 1 - DBL_MIN, 1 - DBL_MIN};

    G4MaterialPropertiesTable *pGlassMPT = new G4MaterialPropertiesTable();
    pGlassMPT->AddProperty("RINDEX", dGlassEnergy, dGlassRefractiveIndex, nGlass);
    pGlassMPT->AddProperty("ABSLENGTH", dGlassEnergy, dGlassAbsorbtionLength, nGlass);
    pGlassMPT->AddProperty("REFLECTIVITY", dGlassEnergy, dGlassReflectivity, nGlass);
    pGlassMPT->AddProperty("TRANSMITTANCE", dGlassEnergy, dGlassTransmittance, nGlass);
    G4cout << "--------------------> Glass MPT" << G4endl;
    pGlassMPT->DumpTable();

    Glass->SetMaterialPropertiesTable(pGlassMPT);
}
