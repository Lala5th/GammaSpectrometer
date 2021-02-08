#ifndef Run_hh
#define Run_hh
#include "G4Run.hh"

#include "G4THitsMap.hh"
#include "globals.hh"

#include "Params.hh"

#include <vector>
#include <mutex>

class G4Event;

class Run : public G4Run{
    public:
        Run();
        ~Run();
        void RecordEvent(const G4Event*);
        void Merge(const G4Run*);
        G4double GetTotalE(G4int i, G4int j){ return Run::GetTotal(fMapSum[i][j][0]); }
        G4double GetTotalNGamma(G4int i, G4int j){ return Run::GetTotal(fMapSum[i][j][1]); }
        void pushEventBack(G4THitsMap<G4double>[ndet_Y][ndet_Z][2]);
        std::vector<G4THitsMap<G4double>(*)[ndet_Z][2]> dumpEventData() const;
        static G4double GetTotal(const G4THitsMap<G4double> &map);
    private:
        G4THitsMap<G4double> fMapSum[ndet_Y][ndet_Z][2];
        G4int fColID[ndet_Y][ndet_Z][2];
        std::vector<G4THitsMap<G4double>(*)[ndet_Z][2]> eventMaps;
        std::mutex evtMutex;

};



#endif
