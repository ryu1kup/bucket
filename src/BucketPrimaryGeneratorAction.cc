#include "BucketPrimaryGeneratorAction.hh"
#include <G4TransportationManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4PhysicalConstants.hh>
#include <Randomize.hh>

BucketPrimaryGeneratorAction::BucketPrimaryGeneratorAction() : m_pParticleGun(nullptr) {
    m_pParticleGun = new G4ParticleGun();
}

BucketPrimaryGeneratorAction::~BucketPrimaryGeneratorAction(){
    delete m_pParticleGun;
}

void BucketPrimaryGeneratorAction::GeneratePrimaries(G4Event *pEvent){
    // laser beam spreads
    const G4ThreeVector hMomentumAxis = m_pParticleGun->GetParticleMomentumDirection();
    constexpr float spread = 7.5 * deg;
    G4ThreeVector hDirection(0, 0, 1);
    hDirection.rotate(spread * G4UniformRand(), G4ThreeVector(0, 1, 0));
    hDirection.rotate(twopi * G4UniformRand(), G4ThreeVector(0, 0, 1));
    hDirection.rotate(hMomentumAxis.theta(), G4ThreeVector(0, 1, 0));
    hDirection.rotate(hMomentumAxis.phi(), G4ThreeVector(0, 0, 1));
    m_pParticleGun->SetParticleMomentumDirection(hDirection);

    // linear hPolarization
    G4ThreeVector hPolarization(1, 0, 0);
    hPolarization.rotate(twopi * G4UniformRand(), G4ThreeVector(0, 0, 1));
    hPolarization.rotate(hDirection.theta(), G4ThreeVector(0, 1, 0));
    hPolarization.rotate(hDirection.phi(), G4ThreeVector(0, 0, 1));
    m_pParticleGun->SetParticlePolarization(hPolarization);

    // generate a primary particle
    m_pParticleGun->GeneratePrimaryVertex(pEvent);
}
