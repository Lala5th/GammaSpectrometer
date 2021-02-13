#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "StackingAction.hh"

void ActionInitialization::Build() const {
  SetUserAction(new PrimaryGeneratorAction);
  SetUserAction(new RunAction);
  SetUserAction(new EventAction);
  SetUserAction(new StackingAction);
}

void ActionInitialization::BuildForMaster() const{
    G4UserRunAction* run_action = new RunAction;
    SetUserAction(run_action);
}

ActionInitialization::ActionInitialization() : G4VUserActionInitialization(){}
