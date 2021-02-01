#include "DetectorMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcommand.hh"
#include "G4ThreeVector.hh"

#include "DetectorConstruction.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* detConst) : fObject(detConst){
    fDirectory = new G4UIdirectory("/det/");
    fDirectory->SetGuidance("Commands to change properties of the detector array. Don't forget to refresh geometry!");
    changeDetLocation = new G4UIcmdWith3VectorAndUnit("/det/setPos",this);
    changeDetLocation->SetGuidance("Changes the position of the CsI array");
    changeDetLocation->SetUnitCategory("Length");
}

DetectorMessenger::~DetectorMessenger(){
    delete fDirectory;
    delete changeDetLocation;
}

void DetectorMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue){
    if(cmd==changeDetLocation){
        G4ThreeVector newPos = changeDetLocation->GetNew3VectorValue(newValue);
        fObject->setDetectorPos(newPos);
    }
}
