#ifndef EventAction_hh
#define EventAction_hh

#include "G4UserEventAction.hh"
#include "G4THitsMap.hh"
#include "globals.hh"

#include "Params.hh"

class EventAction : public G4UserEventAction{
    public:
        EventAction();
        ~EventAction();
        void BeginOfEventAction(const G4Event*);
        void EndOfEventAction(const G4Event*);
        void RecordEvent(const G4Event*);
    private:
        G4THitsMap<G4double> fMapSum[ndet_Y][ndet_Z][2];
        //std::vector<G4THitsMap<G4double>[ndet_Y][ndet_Z][2]> currEvtMap;
        G4int fColID[ndet_Y][ndet_Z][2];
};

#endif
