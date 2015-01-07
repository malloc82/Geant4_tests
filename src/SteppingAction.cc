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
#include "SteppingAction.hh"
#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4SteppingManager.hh"
#include "G4VTouchable.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"
#include "G4StepPoint.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "TrackInformation.hh"
#include "myglobals.hh"
#include "Analysis.hh"

SteppingAction::SteppingAction(EventAction* eventAction):G4UserSteppingAction(),
  fEventAction(eventAction),
  fScoringVolume(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void SteppingAction::UserSteppingAction(const G4Step* step)
{  
  G4StepPoint* preStep = step -> GetPreStepPoint();
  G4StepPoint* postStep = step -> GetPostStepPoint();
  
  if (postStep->GetStepStatus() == fGeomBoundary){
    
    G4int preVolumeCopyN = preStep-> GetPhysicalVolume()-> GetCopyNo();
    G4int postVolumeCopyN = postStep-> GetPhysicalVolume()-> GetCopyNo();
    G4String postVolumeName = postStep-> GetPhysicalVolume()-> GetName();
    
    //if patricle is entering the voxel
    if(preVolumeCopyN != postVolumeCopyN && postVolumeName=="Voxel"){
      
      G4Track* theTrack = step->GetTrack();
      TrackInformation* track_info = (TrackInformation*)(theTrack->GetUserInformation());
      if (track_info != 0) {
	G4String particleName = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
	//G4cout << particleName << " coming from neutron " << step-> GetTrack()->GetCreatorProcess()->GetProcessName() <<endl;
	    
	//GET PHASE SPACE INFO
	    
	//translation of volumes wrt to their mom (phantom)
	G4ThreeVector VoxelCoord=  postStep-> GetPhysicalVolume()->GetTranslation();
	G4double x_voxel = VoxelCoord.x()/um;
	G4double y_voxel = VoxelCoord.y()/um;
	G4double z_voxel = VoxelCoord.z()/um;
	    
	//G4cout << "neutron voxel " <<x_voxel<< " " <<y_voxel<< " " <<z_voxel << " " << postVolumeCopyN <<endl;
	    
	//translation of phantom wrt to their World
	G4ThreeVector VoxelRowCoord= postStep->GetTouchable()->GetVolume(1)->GetTranslation();
	G4double x_row = VoxelRowCoord.x()/um;
	G4double y_row = VoxelRowCoord.y()/um;
	G4double z_row = VoxelRowCoord.z()/um;
	    
	//G4cout << "neutron row " <<x_row<< " " <<y_row<< " " <<z_row << " " << postStep->GetTouchable()->GetVolume(1)->GetCopyNo()<<endl;
      
	//calculate voxel center coordinate wrt to the world
	G4double x_voxel_W = x_voxel+x_row;
	G4double y_voxel_W = y_voxel+y_row;
	G4double z_voxel_W = z_voxel+z_row;
   
	G4double x_post=postStep->GetPosition().x()/um;
	G4double y_post=postStep->GetPosition().y()/um;
	G4double z_post=postStep->GetPosition().z()/um;
	    
	//G4cout <<"neutron before "<< x_post <<" "<< y_post << " "<<z_post<<G4endl;
	    
	secondary_info.name=particleName;
	secondary_info.x=(x_post-x_voxel_W);
	secondary_info.y=(y_post-y_voxel_W);
	secondary_info.z=(z_post-z_voxel_W);
	secondary_info.energy=postStep->GetKineticEnergy()/MeV;
	secondary_info.px=(postStep->GetMomentumDirection().x());
	secondary_info.py=(postStep->GetMomentumDirection().y());
	secondary_info.pz=(postStep->GetMomentumDirection().z());

	//G4cout <<"neutron after "<< secondary_info.x/1000 <<" "<< secondary_info.y/1000 << " "<<secondary_info.z/1000<<G4endl;
	//G4cout << particleName<< " from neutron "<< secondary_info.px <<" "<< secondary_info.py << " "<<secondary_info.pz << " " << secondary_info.energy <<G4endl;
	   
	fEventAction->WriteSecondaryInfo(secondary_info);
	fEventAction->AddParticleCount(1);
	    
	// get analysis manager
// 	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
// 
// 	    // fill ntuple
// 	    //analysisManager->FillNtupleDColumn(0, secondary_info.name);
// 	    analysisManager->FillNtupleDColumn(1, secondary_info.x);
// 	    analysisManager->FillNtupleDColumn(2, secondary_info.y);
// 	    analysisManager->FillNtupleDColumn(3, secondary_info.z);
// 	    analysisManager->FillNtupleDColumn(4, secondary_info.energy);
// 	    analysisManager->FillNtupleDColumn(5, secondary_info.px);
// 	    analysisManager->FillNtupleDColumn(6, secondary_info.py);
// 	    analysisManager->FillNtupleDColumn(7, secondary_info.pz);
// 	    analysisManager->AddNtupleRow();    
// 	    	
      }  
      
    } 
  }      
     
  if (!fScoringVolume) { 
    const DetectorConstruction* detectorConstruction
    = static_cast<const DetectorConstruction*>
    (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    fScoringVolume = detectorConstruction->GetScoringVolume();   
  }
   
  G4LogicalVolume* preVolume = preStep->GetTouchableHandle()->GetVolume()->GetLogicalVolume();
    
  if (preVolume==fScoringVolume){
    G4Track* theTrack = step->GetTrack();
    TrackInformation* track_info = (TrackInformation*)(theTrack->GetUserInformation());
    if (track_info != 0) {
      
      G4int CopyN = preStep-> GetPhysicalVolume()-> GetCopyNo();
      fEventAction->AddEdepStepping(step->GetTotalEnergyDeposit()/MeV,CopyN-1);
      //G4cout << "coming from neutron *** dose " << step->GetTotalEnergyDeposit()/MeV << endl;
    }
  }
    
}