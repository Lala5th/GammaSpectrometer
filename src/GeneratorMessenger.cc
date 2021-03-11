#include "GeneratorMessenger.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcommand.hh"
#include "G4ThreeVector.hh"

#include "PrimaryGeneratorAction.hh"

GeneratorMessenger::GeneratorMessenger(PrimaryGeneratorAction* primaryGeneratorAction) : fObject(primaryGeneratorAction){
    fDirectory = new G4UIdirectory("/mangling/");
    setStd = new G4UIcmdWithADouble("/mangling/setStd",this);
	setExponentCorr = new G4UIcmdWithADouble("/mangling/setEC",this);
}

GeneratorMessenger::~GeneratorMessenger(){
    delete fDirectory;
    delete setStd;
	delete setExponentCorr;
}

void GeneratorMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue){
    if(cmd==setStd){
        G4double newStd = setStd->GetNewDoubleValue(newValue);
        fObject->setStd(newStd);
		return;
    }else if(cmd == setExponentCorr){
		G4double newEC = setExponentCorr->GetNewDoubleValue(newValue);
		fObject->setExponentCorr(newEC);
		return;
	}
}
