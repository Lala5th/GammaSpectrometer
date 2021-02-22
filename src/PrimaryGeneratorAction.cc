#include "PrimaryGeneratorAction.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "Randomize.hh"
#include "G4PhysicalConstants.hh"

#include <functional>
#define _USE_MATH_DEFINES
#include <cmath>

#include "Params.hh"
#include "NumpyAnalysisManager.hh"

#define C_ECrit 150*MeV

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

    NumpyAnalysisManager* man = NumpyAnalysisManager::GetInstance();

    std::function<double(double)> func = [](double E){
        return pow(M_E,-E/C_ECrit)*cbrt(pow(E,-2)); // Why can't pow hande fractional powers?!?!
    };

    G4ThreeVector direction = G4ThreeVector(0,0,1);
    fParticleGun->SetParticlePosition((*initPos));
    fParticleGun->SetParticleMomentumDirection(direction);

    G4double E;
    G4double y;
    double func_val;
    G4int generatedParticles = 0;
    do{
        do{
            E = G4UniformRand()*2000*MeV;
            y = G4UniformRand();
            func_val = func(E);
        }while(y >=func_val);

        fParticleGun->SetParticleEnergy(E);
        man->AddData<float>(3,E);
        fParticleGun->GeneratePrimaryVertex(anEvent);
        generatedParticles++;

    }while(generatedParticles < pNum);
}
