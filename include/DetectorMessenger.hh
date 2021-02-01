#ifndef DetectorMessenger_hh
#define DetectorMessenger_hh
#include "G4UImessenger.hh"

#include "globals.hh"

class DetectorConstruction;
class G4UIdirectory;
class G4UIcmdWith3VectorAndUnit;
class G4UIcommand;

class DetectorMessenger : public G4UImessenger{
    public:
        DetectorMessenger(DetectorConstruction*);
        ~DetectorMessenger();
        void SetNewValue(G4UIcommand* command, G4String newValue) override;
    private:
        DetectorConstruction* fObject;
        G4UIdirectory* fDirectory;
        G4UIcmdWith3VectorAndUnit* changeDetLocation;
};

#endif
