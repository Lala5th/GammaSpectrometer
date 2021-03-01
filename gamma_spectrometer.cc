#ifndef G4MULTITHREADED
#include "G4RunManager.hh"
#endif
#ifdef G4MULTITHREADED // Don't ask why but it doesn't compile with ifdef else
#include "G4MTRunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4VisManager.hh"
#include "G4UIExecutive.hh"
#include "G4VModularPhysicsList.hh"
#include "G4OpticalPhysics.hh"
#include "FTFP_BERT.hh"
#include "G4SDManager.hh"
#include "G4EmStandardPhysics_option4.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"
#include "Run.hh"
#include "RunAction.hh"

#include "NumpyAnalysisManager.hh"

int main(int argc, char *argv[]){
    // construct the default run manager
#ifdef G4MULTITHREADED
    G4MTRunManager* runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(8);
#else
    G4RunManager* runManager = new G4RunManager;
#endif
    G4VisManager* visManager = new G4VisExecutive;

    // Init analysis manager
    NumpyAnalysisManager* man = NumpyAnalysisManager::GetInstance();
    man->CreateDataset<int>("y"); // ID:0
    man->CreateDataset<int>("z"); // 1
    man->CreateDataset<int,float>("DepositedEnergy"); // 2
    man->CreateDataset<int,double>("Energy"); // 3
    man->CreateDataset<double>("ECrit"); // 4

    // Create Y-Z map
    for(int i = 0; i < ndet_Y; i++)
        man->AddData<int>(0,i);
    for(int f = 0; f < ndet_Z; f++)
        man->AddData<int>(1,f);

    // set mandatory initialization classes
    runManager->SetUserInitialization(new DetectorConstruction);
    runManager->SetUserInitialization(new PhysicsList);
    runManager->SetUserInitialization(new ActionInitialization);

    // initialize G4 kernel
    runManager->Initialize();
    visManager->Initialize();

    // get the pointer to the UI manager and set verbosities
    G4UImanager* UI = G4UImanager::GetUIpointer();
    #ifdef VERBOSE
    UI->ApplyCommand("/run/verbose 1");
    UI->ApplyCommand("/event/verbose 1");
    UI->ApplyCommand("/tracking/verbose 1");
    #endif
    G4UIExecutive* ui = 0;
    if (argc == 1) ui = new G4UIExecutive(argc,argv);
    if ( ! ui ) {
        // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];

        UI->ApplyCommand(command+fileName);
    }
    else {
        // interactive mode

        UI->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }

    // start a run
    /*#ifndef VIS
    int numberOfEvent = 8;
    int numberOfRuns = 1;
    for(G4int i = 0; i < numberOfRuns; i++){
        runManager->BeamOn(numberOfEvent);
        RunAction::IncrementRun();
    }
    #endif*/

    // job termination
    delete runManager;
    delete visManager;
    return 0;
}
