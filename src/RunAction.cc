#include "RunAction.hh"

#include "Run.hh"

#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4RegionStore.hh"
#include "G4UnitsTable.hh"
#include "G4ParticleGun.hh"

#include "NumpyAnalysisManager.hh"
#include "Params.hh"

G4int RunAction::ID = 0;
// Hack because they use the same messenger.
G4ParticleGun* RunAction::particleSource = new G4ParticleGun(1);

RunAction::RunAction() : G4UserRunAction(){}
RunAction::~RunAction(){}

G4Run* RunAction::GenerateRun(){ return new Run; }

void RunAction::BeginOfRunAction(const G4Run*){}

void RunAction::EndOfRunAction(const G4Run* aRun){
    if(IsMaster()){
        NumpyAnalysisManager* man = NumpyAnalysisManager::GetInstance();
        Run* run = (Run*) aRun;
        G4double eventN = (G4double) run->GetNumberOfEvent();
        G4cout << "######################################################################" << G4endl;
        G4cout << "Run " << RunAction::ID << " Summary - " << eventN << " Events Processed" << G4endl;
        G4cout << "######################################################################" << G4endl;
        G4String name;
        #ifdef VERBOSE
        G4cout << "Name\t\tEdep\t\tnGamma" << G4endl;
        #endif
        std::vector<G4THitsMap<G4double>(*)[ndet_Z][2]> eventMaps = run->dumpEventData();
        for(G4int i = 0; i < ndet_Y; i++){
            for(G4int f = 0; f < ndet_Z; f++){
                name = "Detector_";
                name += std::to_string(i);
                name += "|";
                name += std::to_string(f);
                //G4Region* detector = G4RegionStore::GetInstance()->GetRegion(name);
                man->AddData<int,float>(2,RunAction::ID,run->GetTotalE(i, f)/(eventN*pNum));
                #ifdef VERBOSE
                G4cout << name << "\t" << G4BestUnit(run->GetTotalE(i,f),"Energy") << "\t" << run->GetTotalNGamma(i, f) << G4endl;
                #endif
            }
        }
        RunAction::IncrementRun();
    }
}
