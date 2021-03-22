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

void Run::PurgeEvents(){
    for(size_t i = 0; i < perEvt.size();i++){
        delete[] perEvt.at(i);
    }
}

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
    std::vector<G4double(*)[ndet_Z]> perEvts = run->dumpPerEvts();
    for(size_t i = 0;i < perEvts.size();i++)
        perEvt.push_back(perEvts.at(i));
    G4Run::Merge(aRun);
}

std::vector<G4double(*)[ndet_Z]> Run::dumpPerEvts() const{
    return perEvt;
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

std::vector<G4double> Run::GetTotalStd(const std::vector<G4double(*)[ndet_Z]> &perEvt,int i,int j){
    G4double tot = 0.;
    int evtNum = perEvt.size();
    double mean = 0;
    for (auto& evt : perEvt){
        mean += evt[i][j];
        //G4cout << evt[i][j] << "\n";
    }
    mean /= evtNum;
    for (auto& evt : perEvt)
        tot += pow(evt[i][j]-mean,2);
    //G4cout << perEvt.at(97)[i][j] << "|" << perEvt.at(98)[i][j] << "|" << perEvt.at(99)[i][j] << G4endl;
    //G4cout << pow(tot/evtNum,0.5) << "|" << mean << G4endl;
    return {tot/(evtNum-1),mean};
}

void Run::AddPerEvt(G4HCofThisEvent* HCE){
    auto val = new double[ndet_Y][ndet_Z];
    G4THitsMap<G4double> mapSum[ndet_Y][ndet_Z][2];
    for(G4int i = 0; i < ndet_Y; i++){
        for(G4int f = 0; f < ndet_Z; f++){
            for(G4int k = 0; k < 2; k++){
                G4THitsMap<G4double>* evtMap = (G4THitsMap<G4double>*)(HCE->GetHC(fColID[i][f][k]));
                mapSum[i][f][k] += *evtMap;
            }
        }
    }
    for(int i = 0; i < ndet_Y;i++){
        for(int f = 0; f < ndet_Z;f++){
            val[i][f] = 0;
            std::map<G4int,G4double*>::iterator itr = mapSum[i][f][0].GetMap()->begin();
            for(; itr != mapSum[i][f][0].GetMap()->end(); itr++)
            { val[i][f] += *(itr->second); }
        }
    }
    this->perEvt.push_back(val);
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
