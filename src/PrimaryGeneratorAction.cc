#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"

#include "Params.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(const G4String& particleName,
                                               G4double energy,
                                               G4ThreeVector position,
                                               G4ThreeVector momentumDirection)
                                               : G4VUserPrimaryGeneratorAction(){

    fParticleGun = new G4ParticleGun(1);
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(energy);
    //fParticleGun->SetParticlePosition(position);
    fParticleGun->SetParticleMomentumDirection(momentumDirection);
    initPos = new G4ThreeVector(position);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction(){
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){
    for(int i = 0; i < pNum; i++){
        //G4double phi = G4UniformRand()*360;
        //G4double theta = (G4UniformRand() - 0.5)*0.003;
        G4ThreeVector direction = G4ThreeVector(0,0,1);
        //direction.rotateY(theta*rad);
        //direction.rotateX(phi*deg);
        //phi = G4UniformRand()*360;
        //G4double offset = G4UniformRand()*1;
        //G4ThreeVector randOffset = G4ThreeVector(0,0,offset);
        //randOffset.rotateX(phi*deg);
        fParticleGun->SetParticlePosition((*initPos));
        fParticleGun->SetParticleMomentumDirection(direction);
        fParticleGun->GeneratePrimaryVertex(anEvent);
    }
}
