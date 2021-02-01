#ifndef PrimaryGeneratorAction_hh
#define PrimaryGeneratorAction_hh
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"

class G4ParticleGun;
class G4Event;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction{
    public:
        PrimaryGeneratorAction(const G4String& particleName = "gamma",
                               G4double energy = 30*MeV,
                               G4ThreeVector position= G4ThreeVector(0,0,-2*m),
                               G4ThreeVector momentumDirection = G4ThreeVector(0,0,1));
        ~PrimaryGeneratorAction();
        virtual void GeneratePrimaries(G4Event*);
    private:
        G4ParticleGun* fParticleGun;
        G4ThreeVector* initPos;
};

#endif
