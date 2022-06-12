#include <iostream>

#include "G4UImanager.hh"
#include "G4VisManager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "construction.hh"
#include "physics.hh"
#include "action.hh"

int main(int argc, char **argv)
{
    G4UIExecutive *ui = 0;

#ifdef G4MULTITHREADED
    G4MTRunManager *runManager = new G4MTRunManager;
    runManager -> SetNumberOfThreads(1);
#else
    G4RunManager *runManager = new G4RunManager;

#endif

    runManager->SetUserInitialization(new MyDetectorConstruction()); // Geometry
    runManager->SetUserInitialization(new MyPhysicsList());          // Physics List
    runManager->SetUserInitialization(new MyActionInitialization()); // run details include source generator and scoring

    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    G4VisManager *visManager = new G4VisExecutive();
    visManager->Initialize();

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (ui) // if ui is true, create graphic
    {

        UImanager->ApplyCommand("/control/execute vis.mac");

        ui->SessionStart();
    }
    else
    {
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }

    return 0;
}
