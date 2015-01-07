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
// -------------------------------------------------------------------
// $Id$
// -------------------------------------------------------------------

#include "RunAction.hh"
#include "G4Run.hh"
#include "G4ParticleDefinition.hh"
#include "EventAction.hh"
#include "Run.hh"
#include "G4RunManager.hh"

using namespace std;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

RunAction::RunAction(G4String name_file)
  : G4UserRunAction(), 
  fNameFileOut(name_file)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

RunAction::~RunAction()
{}

G4Run* RunAction::GenerateRun()
{
  return new Run((fNameFileOut)); 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void RunAction::BeginOfRunAction(const G4Run*)
{  
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}
 
void RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
  
  Run* theRun = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  
  // tot energy deposited voxel ring
  G4double* edep  = theRun->GetEdep();
  G4double* edep2 = theRun->GetEdep2();
//   G4double rms_v = edep2 - edep*edep/nofEvents;
//   if (rms_v > 0.) rms_v = std::sqrt(rms_v); else rms_v = 0.;
  
 
  G4String fileName;
  fileName = "../phase_space/";
  fileName.append("edep_");
  fileName.append(fNameFileOut);
    
  if (IsMaster())
  {
    
    ofstream outfile_edep;
    outfile_edep.open (fileName, fstream::app);
    for(G4int i=0; i<NUM_VOXELS; i++){
    
      outfile_edep <<edep[i]<< " "<<edep2[i]<< " " <<G4endl;
      
    }
    outfile_edep.close();
    
    G4cout
     << "\n--------------------End of Global Run-----------------------"
     << " \n The run was " << nofEvents << " events ";
  }
  else
  {
    G4cout
     << "\n--------------------End of Local Run------------------------"
     << " \n The run was " << nofEvents ;
  }      
  
  G4cout
     << "\n Total edep in voxel : " 
     << "\n------------------------------------------------------------\n"
     << G4endl;
  
  
  
  
  
  
  
    
}
