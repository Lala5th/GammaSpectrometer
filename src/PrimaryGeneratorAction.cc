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
#include <random>

#include "Params.hh"
#include "NumpyAnalysisManager.hh"
#include "GeneratorMessenger.hh"

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
    genMessenger = new GeneratorMessenger(this);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction(){
    delete fParticleGun;
    delete genMessenger;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent){

    NumpyAnalysisManager* man = NumpyAnalysisManager::GetInstance();
    double ECrit = fParticleGun->GetParticleEnergy();
    std::mt19937_64 randGen = std::mt19937_64();
    std::normal_distribution<double> dist(0,this->std);
    man->AddData<double,double>(5,this->std,this->expCorr);

    std::function<double(double)> func = [this,ECrit](double E){
        return pow(M_E,-E/ECrit)*pow(E,((double) -2)/3 + this->expCorr);
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
            E = G4UniformRand()*1000*MeV;
            y = G4UniformRand()*10;
            func_val = func(E);
        }while(y >=func_val);
        E += dist(randGen);
        if(E <= 0)
            continue;

        fParticleGun->SetParticleEnergy(E);
        man->AddData<int,double>(3,generatedParticles,E);
        fParticleGun->GeneratePrimaryVertex(anEvent);
        generatedParticles++;

    }while(generatedParticles < pNum);
}
