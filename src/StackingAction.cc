#include "StackingAction.hh"

#include "G4Track.hh"
#include "G4OpticalPhoton.hh"


StackingAction::StackingAction(){}

StackingAction::~StackingAction(){}


G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* track){

	int parentID = track->GetParentID();

	if (track->GetDefinition() == G4OpticalPhoton::Definition()) return fKill;
	if (parentID < 0) return fKill;
	return fUrgent;
}
