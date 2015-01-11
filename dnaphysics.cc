//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// This example is provided by the Geant4-DNA collaboration
// Any report or published results obtained using the Geant4-DNA software 
// shall cite the following Geant4-DNA collaboration publication:
// Med. Phys. 37 (2010) 4692-4708
// The Geant4-DNA web site is available at http://geant4-dna.org
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

#ifdef G4MULTITHREADED
  #include "G4MTRunManager.hh"
#else
  #include "G4RunManager.hh"
#endif

#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4UIExecutive.hh"

#ifdef G4VIS_USE
  #include "G4VisExecutive.hh"
#endif

#include "ActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "PhysicsList.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

int main(int argc,char** argv) 
{
  // Choose the Random engine
  time_t start_time = time(0);
  pid_t pid = getpid();
  G4int seed = start_time * (pid << 16);
  
  G4Random::setTheEngine(new CLHEP::RanecuEngine);
  G4Random::setTheSeed(10);
 
  G4cout<< "**** Seed ****"<< G4endl;
  G4cout << "seed "<< seed << G4endl;
  G4cout << "getTheSeed " << G4Random::getTheSeed() << G4endl;
  G4cout<< "*************"<< G4endl;
 
  // Construct the default run manager

#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager;
  //runManager->SetNumberOfThreads(3);
#else
G4RunManager* runManager = new G4RunManager;  
#endif

  
  // Set mandatory user initialization classes
  
  DetectorConstruction* detector = new DetectorConstruction;
  runManager->SetUserInitialization(detector);  
  runManager->SetUserInitialization(new PhysicsList);
  
  G4String NameFile=argv[2];
  runManager->SetUserInitialization(new ActionInitialization(detector,NameFile));
  
  // Initialize G4 kernel  
  runManager->Initialize();

#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif
    
  // Get the pointer to the User Interface manager 
  
  G4UImanager* UI = G4UImanager::GetUIpointer(); 

  if (argc!=1) // batch mode  
    { 
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UI->ApplyCommand(command+fileName);  
    }
  
  else  // define visualization and UI terminal for interactive mode 
    { 
#ifdef G4UI_USE
     G4UIExecutive * ui = new G4UIExecutive(argc,argv);      
#ifdef G4VIS_USE
     UI->ApplyCommand("/control/execute vis.mac");          
#endif
     if (ui->IsGUI())
         UI->ApplyCommand("/control/execute gui.mac");
     ui->SessionStart();
     delete ui;
#endif
    }

#ifdef G4VIS_USE
  delete visManager;
#endif

  delete runManager;

  return 0;
}

