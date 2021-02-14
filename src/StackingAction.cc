#include "StackingAction.hh"

#include "G4Track.hh"
#include "G4OpticalPhoton.hh"


StackingAction::StackingAction(){}

StackingAction::~StackingAction(){}


G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* track){

	if (track->GetDefinition() == G4OpticalPhoton::Definition()) return fKill;
	return fUrgent;
}
