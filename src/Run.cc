#include "Run.hh"

#include "G4Run.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "globals.hh"

Run::Run() : G4Run(){
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

Run::~Run(){}

void Run::Merge(const G4Run* aRun){
    const Run* run = static_cast<const Run*>(aRun);
    for(G4int i = 0; i < ndet_Y; i++){
        for(G4int f = 0; f < ndet_Z; f++){
            for(G4int k = 0; k < 2; k++){
                fMapSum[i][f][k] += run->fMapSum[i][f][k];
            }
        }
    }
    std::vector<G4THitsMap<G4double>(*)[ndet_Z][2]> evtData = run->dumpEventData();
    for(auto itr = evtData.begin();itr != evtData.end();itr++){
        pushEventBack(*itr);
    }
    G4Run::Merge(aRun);
}

void Run::RecordEvent(const G4Event *event){
    G4HCofThisEvent* HCE = event->GetHCofThisEvent();
    if(!HCE) return;
    numberOfEvent++;
    for(G4int i = 0; i < ndet_Y; i++){
        for(G4int f = 0; f < ndet_Z; f++){
            for(G4int k = 0; k < 2; k++){
                G4THitsMap<G4double>* evtMap = (G4THitsMap<G4double>*)(HCE->GetHC(fColID[i][f][k]));
                fMapSum[i][f][k] += *evtMap;

            }
        }
    }

}

G4double Run::GetTotal(const G4THitsMap<G4double> &map){
    G4double tot = 0.;
    if(map.GetSize()==0) return tot;
    std::map<G4int,G4double*>::iterator itr = map.GetMap()->begin();
    for(; itr != map.GetMap()->end(); itr++)
    { tot += *(itr->second); }
    return tot;
}

void Run::pushEventBack(G4THitsMap<G4double> eventMap[ndet_Y][ndet_Z][2]){
    evtMutex.lock();
    eventMaps.push_back(eventMap);
    evtMutex.unlock();
}

std::vector<G4THitsMap<G4double>(*)[ndet_Z][2]> Run::dumpEventData() const{
    //evtMutex.lock();
    std::vector<G4THitsMap<G4double>(*)[ndet_Z][2]> copy = eventMaps;
    //evtMutex.unlock();
    return copy;
}
