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
// $Id: B1Run.cc 66536 2012-12-19 14:32:36Z ihrivnac $
//
/// \file B1Run.cc
/// \brief Implementation of the B1Run class

#include "Run.hh"
#include <iostream>
#include "G4Threading.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Run::Run(G4String file_name)
: G4Run(),
  fPrintModulo(100),
  fNofParticles(0),
  fFileName(file_name)

{
    fill( fEdep, fEdep + NUM_VOXELS , 0 );
    fill( fEdep2, fEdep2 + NUM_VOXELS , 0 );
    fsecondaryInfo.clear();
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

Run::~Run()
{} 

void Run::RecordEvent(const G4Event* event)
{
  G4int evtNb = event->GetEventID();
  
  if (evtNb%fPrintModulo == 0) {G4cout << "\n---> end of event: " << evtNb << G4endl;}
  
  G4String fileName = "../phase_space/";
  fileName.append("ps_");
  fileName.append(fFileName);
  //fileName.append("_");
  
  ostringstream convert; 
  convert << G4Threading::G4GetThreadId();      
  G4String runIDStr = convert.str();
  
  //fileName.append(runIDStr);
  
  psFile.open (fileName, fstream::app);  
  
  for (int jj=0; jj<fNofParticles; jj++) {
    
    psFile << evtNb << " " << fsecondaryInfo[jj].name << " " << fsecondaryInfo[jj].x << " " << fsecondaryInfo[jj].y << " " << fsecondaryInfo[jj].z
    << " " << fsecondaryInfo[jj].energy << " " << fsecondaryInfo[jj].px << " " << fsecondaryInfo[jj].py << " " << fsecondaryInfo[jj].pz <<endl;
    }
  
   psFile.close();  
   G4Run::RecordEvent(event); 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void Run::Merge(const G4Run* run)
{
  const Run* localRun = static_cast<const Run*>(run);
  for(G4int i=0; i<NUM_VOXELS; i++){
    
    fEdep[i]+=localRun->fEdep[i];
    fEdep2[i]+=localRun->fEdep2[i];
  }
  
  G4Run::Merge(run); 
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void Run::AddEdepEvent (G4double* edep_voxel)
{
  for(G4int i=0; i<NUM_VOXELS; i++){
   
    fEdep[i]  += edep_voxel[i];
    fEdep2[i] += edep_voxel[i]*edep_voxel[i];
    
  } 

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


