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

#include "PhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4RegionStore.hh"
#include "G4UserSpecialCuts.hh"

#include "G4EmStandardPhysics.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronElasticPhysicsHP.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmExtraPhysics.hh"
#include "G4StoppingPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4IonBinaryCascadePhysics.hh"
// #include "MyEmDNAPhysics.hh"


//#include "G4NeutronTrackingCut.hh"
//#include "LocalIonIonInelasticPhysic.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList()
    : G4VModularPhysicsList()
{
    defaultCutValue = 10.*micrometer;
    cutForGamma     = defaultCutValue;
    cutForElectron  = defaultCutValue;
    cutForPositron  = defaultCutValue;
    cutForProton    = defaultCutValue;

    SetVerboseLevel(2);
    //list from Susanna
    // EM physics
    RegisterPhysics(new G4EmLivermorePhysics());
    // Synchroton Radiation & GN Physics
    RegisterPhysics(new G4EmExtraPhysics());
    // Decays
    RegisterPhysics(new G4DecayPhysics());
    // Hadron Physics 
    RegisterPhysics(new G4HadronPhysicsQGSP_BIC_HP());   
    // Hadron Elastic scattering
    RegisterPhysics(new G4HadronElasticPhysicsHP());
    // Stopping Physics
    RegisterPhysics(new G4StoppingPhysics());
    // Ion Physics 
    //RegisterPhysics(new G4IonPhysics());  
    //
    RegisterPhysics(new G4RadioactiveDecayPhysics()); 
    // Inelastic scattering of d, t, alpha, He3, ions (stripped nuclei)
    // Binary cascade model up to 20 GeV.
    RegisterPhysics(new G4IonBinaryCascadePhysics()); 


}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList()
{}


void PhysicsList::SetCuts()
{
    if (verboseLevel >0)
    {
        G4cout << "PhysicsList::SetCuts:";
        G4cout << "CutLength : " << G4BestUnit(defaultCutValue,"Length") << G4endl;
    }

    // set cut values for gamma at first and for e- second and next for e+,
    // because some processes for e+/e- need cut values for gamma
    SetCutValue(defaultCutValue, "gamma");
    SetCutValue(defaultCutValue, "e-");
    SetCutValue(defaultCutValue, "e+");
    SetCutValue(defaultCutValue, "proton");

    G4double lowLimit = 250. * eV;
    //e' il minimo accettabile poi lo posso cambiare con set cut e user limits
    G4double highLimit = 100. * GeV;
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowLimit, highLimit);

    if (verboseLevel>0) {DumpCutValuesTable();}
}
