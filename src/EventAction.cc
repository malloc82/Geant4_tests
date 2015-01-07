#include <algorithm>  // copy()
#include <fstream>    // fstream
#include <iostream>   // cout
#include <iterator>   // istream_iterator, ostream_iterator, back_inserter
#include <sstream>    // istringstream
#include <string>     // string
#include <vector>     // vector
#include <limits>

#include "EventAction.hh"
#include "RunAction.hh"
#include "Run.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"

using namespace std;

EventAction::EventAction()
    : G4UserEventAction(),
      fPrintModulo(100),
      fParticleCount(0)      
{
  Reset();
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{
  Reset();
}

void EventAction::EndOfEventAction(const G4Event* event)
{
  // accumulate statistics in B1Run
  Run* run = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
  
  //accumulate statistics
  run->AddEdepEvent(fEnergySum); 
  
  // transfer event info to Run
  run->WriteSecondaryInfo(fSecondary_info);
  run->ParticleCount(fParticleCount);
     
}

void EventAction::AddEdepStepping(G4double edep, G4int copy) 
{ 
  
  fEnergySum[copy] += edep;  

}

void EventAction::Reset()
{
    //Reset accounted energy in stepping action
    fParticleCount=0;
    fill( fEnergySum, fEnergySum + NUM_VOXELS , 0 );
    fSecondary_info.clear();

}


