#include "EventAction.hh"

#include "Analysis.hh"
#include "RunAction.hh"
#include "Run.hh"

#include "G4RunManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"

#include "NumpyAnalysisManager.hh"

EventAction::EventAction() : G4UserEventAction(){}

EventAction::~EventAction(){}

void EventAction::BeginOfEventAction(const G4Event*){
    G4String name = "Detector";
    G4SDManager* sdm = G4SDManager::GetSDMpointer();
    G4String collection[2] = {"eDep","nGamma"};
    for(G4int i = 0; i < ndet_Y; i++){
        for(G4int f = 0; f < ndet_Z; f++){
            name = "Detector_";
            name += std::to_string(i);
            name += "|";
            name += std::to_string(f);
            fColID[i][f][0] = sdm->GetCollectionID(name+"/"+collection[0]);
            fColID[i][f][1] = sdm->GetCollectionID(name+"/"+collection[1]);
        }
    }
}

void EventAction::EndOfEventAction(const G4Event* event){
    RecordEvent(event);
}

void EventAction::RecordEvent(const G4Event* event){
    G4HCofThisEvent* HCE = event->GetHCofThisEvent();
    NumpyAnalysisManager* man = NumpyAnalysisManager::GetInstance();
    //G4cout << "EventID: " << event->GetEventID() << G4endl;
    if(!HCE){
        for(G4int i = 0; i < ndet_Y; i++)
            for(G4int f = 0; f < ndet_Z; f++)
                man->AddData<int,float>(2,event->GetEventID(),0.);
        return;
    }
    G4double tot = 0.;
    for(G4int i = 0; i < ndet_Y; i++){
        for(G4int f = 0; f < ndet_Z; f++){
            tot = 0;
            for(G4int k = 0; k < 2; k++){
                G4THitsMap<G4double>* evtMap = (G4THitsMap<G4double>*)(HCE->GetHC(fColID[i][f][k]));
                fMapSum[i][f][k] += *evtMap;
            }
            std::map<G4int,G4double*>::iterator itr = fMapSum[i][f][0].GetMap()->begin();
            for(; itr != fMapSum[i][f][0].GetMap()->end(); itr++)
            { tot += *(itr->second); }
            man->AddData<int,float>(2,event->GetEventID(),tot);
        }
    }
    Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    run->pushEventBack(fMapSum);
}
