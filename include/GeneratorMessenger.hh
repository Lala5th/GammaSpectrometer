#ifndef GeneratorMessenger_HH
#define GeneratorMessenger_HH

#include "G4UImessenger.hh"

#include "globals.hh"

class PrimaryGeneratorAction;
class G4UIdirectory;
class G4UIcmdWithADouble;
class G4UIcommand;

class GeneratorMessenger : public G4UImessenger{
    public:
        GeneratorMessenger(PrimaryGeneratorAction*);
        ~GeneratorMessenger();
        void SetNewValue(G4UIcommand* command, G4String newValue) override;
    private:
        PrimaryGeneratorAction* fObject;
        G4UIdirectory* fDirectory;
        G4UIcmdWithADouble* setStd;
        G4UIcmdWithADouble* setExponentCorr;
};


#endif
