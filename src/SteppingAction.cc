#include "SteppingAction.hh"

#include "globals.hh"
#include "G4Step.hh"
#include "G4OpticalPhoton.hh"

#include "EventAction.hh"
#include "DetectorConstruction.hh"

SteppingAction::SteppingAction()
  : G4UserSteppingAction(){}

SteppingAction::~SteppingAction(){}

void SteppingAction::UserSteppingAction(const G4Step* step){
	G4Track* track = step->GetTrack();
	G4VPhysicalVolume* nextVol = track->GetNextVolume();
	if(nextVol == nullptr){
		track->SetTrackStatus(G4TrackStatus::fStopAndKill);
	}else if(nextVol->GetName() == "World"){
		track->SetTrackStatus(G4TrackStatus::fStopAndKill);
	}
	if(track->GetParticleDefinition() == G4OpticalPhoton::Definition()){
		track->SetTrackStatus(G4TrackStatus::fStopAndKill);
	}
}
