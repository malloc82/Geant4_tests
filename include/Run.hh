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
// $Id: B1Run.hh 66536 2012-12-19 14:32:36Z ihrivnac $
//
/// \file B1Run.hh
/// \brief Definition of the Run class

#ifndef Run_h
#define Run_h 1

#include "G4Run.hh"
#include "globals.hh"
#include "Vec.hh"
#include "G4Event.hh"
#include "myglobals.hh"
#include <vector> 
#include <cstdio>
#include <map>
#include <fstream>

using namespace std;


/// Run class
///

class Run : public G4Run
{
  public:
    Run(G4String file_name);
    virtual ~Run();

    // method from the base class
    virtual void Merge(const G4Run*);
    virtual void RecordEvent(const G4Event*);
    
    void AddEdepEvent (G4double* edep);
    void WriteSecondaryInfo(vector <Vec> info) {fsecondaryInfo = info;}
    void ParticleCount(G4int number){fNofParticles = number;}
    
    // get methods
    G4double* GetEdep()  { return fEdep; }
    G4double* GetEdep2() { return fEdep2; }

  private:
    G4int fPrintModulo;
    G4int fNofParticles;
   
    G4double  fEdep[NUM_VOXELS+1];
    G4double  fEdep2[NUM_VOXELS+1];
     
    G4String fFileName;
    vector <Vec> fsecondaryInfo;
    ofstream psFile;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

