#ifndef SteppingAction_HH
#define SteppingAction_HH

#include "G4UserSteppingAction.hh"

class G4Step;
class DetectorConstruction;
class EventAction;


class SteppingAction : public G4UserSteppingAction{
	public:
		SteppingAction();
		~SteppingAction();
		void UserSteppingAction(const G4Step*);
	private:
		const DetectorConstruction* fDetConstruction;
		EventAction* fEventAction;
};

#endif
