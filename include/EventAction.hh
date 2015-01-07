#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "Vec.hh"
#include <vector> 
#include "myglobals.hh"

using namespace std;

class SteppingAction;

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
    virtual ~EventAction();

    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);
 
    
    void AddEdepStepping(G4double edep, G4int copy);
    void AddParticleCount(G4double number) {fParticleCount += number; }
    void WriteSecondaryInfo(Vec info) {fSecondary_info.push_back(info);}
    
    void Reset();

  private:
    G4int     fPrintModulo;
    G4int     fParticleCount;
    
    G4double  fEnergySum[NUM_VOXELS+1];
    vector <Vec> fSecondary_info;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
