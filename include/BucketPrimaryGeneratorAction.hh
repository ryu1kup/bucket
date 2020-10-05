#pragma once

#include <G4VUserPrimaryGeneratorAction.hh>
#include <G4Event.hh>
#include <G4ParticleGun.hh>

class BucketPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        BucketPrimaryGeneratorAction();
        ~BucketPrimaryGeneratorAction();

    public:
        void GeneratePrimaries(G4Event *pEvent) override;

    private:
        G4ParticleGun *m_pParticleGun;
};
