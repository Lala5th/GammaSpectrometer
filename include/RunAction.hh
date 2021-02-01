#ifndef RunAction_hh
#define RunAction_hh
#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

#include <vector>

class G4Run;
class G4ParticleGun;

class RunAction : public G4UserRunAction{
    public:
        RunAction();
        ~RunAction();
        virtual void BeginOfRunAction(const G4Run*);
        virtual void EndOfRunAction(const G4Run*);
        G4Run* GenerateRun();
        static void IncrementRun(){ RunAction::ID++; }
        static G4int GetRunID(){ return RunAction::ID; };
    private:
        static G4int ID;
        static G4ParticleGun* particleSource;
};

#endif
