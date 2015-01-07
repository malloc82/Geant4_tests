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

#include "DetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4UserLimits.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4PVParameterised.hh"

#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "SteppingAction.hh"
#include "globals.hh"
#include "myglobals.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DetectorConstruction::DetectorConstruction():    
    fSolidWorld(0), fLogicWorld(0),fPhysicWorld(0),
    fScoringVolume(0)
 
{}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

G4VPhysicalVolume* DetectorConstruction::Construct()

{
  DefineMaterials();
  return ConstructDetector();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void DetectorConstruction::DefineMaterials()
{ 

  // Water is defined from NIST material database
  G4NistManager * man = G4NistManager::Instance();
  G4Material * H2O = man->FindOrBuildMaterial("G4_WATER");
  G4Material * air = man->FindOrBuildMaterial("G4_AIR");

  waterMaterial = H2O;
  airMaterial = air;
  
  //G4cout << "-> Density of water material (g/cm3)=" << fWaterMaterial->GetDensity()/(g/cm/cm/cm) << G4endl;
  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
G4VPhysicalVolume* DetectorConstruction::ConstructDetector()
{

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

//WORLD VOLUME LIVERMORE PHYSICS
    WorldSizeX  = 2.*m;
    WorldSizeY  = 2.*m;
    WorldSizeZ  = 2.*m;

    fSolidWorld = new G4Box("World",				     //its name
                           WorldSizeX/2,WorldSizeY/2,WorldSizeZ/2);  //its size


    fLogicWorld = new G4LogicalVolume(fSolidWorld,	//its solid
                                     airMaterial,	//its material
                                     "World");		//its name

    fPhysicWorld = new G4PVPlacement(0,			//no rotation
                                   G4ThreeVector(),	//at (0,0,0)
                                   "World",		//its name
                                   fLogicWorld,		//its logical volume
                                   0,			//its mother  volume
                                   false,		//no boolean operation
                                   0);			//copy number
    
    //WATER PHANTOM
    G4bool checkOverlaps = true;
    G4double PhantomSizeX = 30.*cm;
    G4double PhantomSizeY = PhantomSizeX;
    G4double PhantomSizeZ = PhantomSizeX;

    G4Box *  solidPhantom = new G4Box("Phantom",PhantomSizeX/2,PhantomSizeY/2,PhantomSizeZ/2);   

    G4LogicalVolume* logicPhantom = new G4LogicalVolume(solidPhantom,waterMaterial,"Phantom");

    new G4PVPlacement(0,	//no rotation
		     G4ThreeVector(),	//at (0,0,0)
		     logicPhantom,
		     "Phantom",		//its name
                     fLogicWorld,
                     false,		//no boolean operation
                     0,checkOverlaps);	//copy number


    //SCORING VOXELS
    G4double VoxelSide =  1*cm;
    G4Box* solidVoxel = new G4Box("Voxel", VoxelSide/2,VoxelSide/2,VoxelSide/2);   
    G4LogicalVolume*  logicVoxel = new G4LogicalVolume(solidVoxel,waterMaterial,"Voxel");
    
    //Compute first the number of points
    G4double radius=10;
    G4int octant_size = floor((sqrt(2)*(radius-1)+4)/2);
    
    //G4cout << octant_size << endl;
    G4int n_points = octant_size*8;
    
    //Iterate a second time, and this time retrieve coordinates.
    //We "zig-zag" through indices, so that we reconstruct a continuous
    //set of of x,y coordinates, starting from the top of the circle.
    G4double x0 = 0;
    G4double y0 = 0;
    G4double x = 0;
    G4double y = radius;
     
    G4double f = 1 - radius;
    G4double dx = 1;
    G4double dy = - 2 * radius;
     
    vector <G4double> xc (n_points);
    vector <G4double> yc (n_points);
     
    //1 octant
    xc.at(0) = x0 + x;
    yc.at(0) = y0 + y;

    //2nd octant 
    xc.at(8 * octant_size-1) = x0 - x;
    yc.at(8 * octant_size-1) = y0 + y;
     
    //3rd octant 
    xc.at(4 * octant_size-1) = x0 + x;
    yc.at(4 * octant_size-1) = y0 - y;
     
    //4th octant 
    xc.at(4 * octant_size) = x0 - x;
    yc.at(4 * octant_size) = y0 - y;
     
    //5th octant 
    xc.at(2 * octant_size-1) = x0 + y;
    yc.at(2 * octant_size-1) = y0 + x;
     
    //6th octant 
    xc.at(6 * octant_size) = x0 - y;
    yc.at(6 * octant_size) = y0 + x;
     
    //7th octant 
    xc.at(2 * octant_size) = x0 + y;
    yc.at(2 * octant_size) = y0 - x;
     
    //8th octant 
    xc.at(6 * octant_size-1) = x0 - y;
    yc.at(6 * octant_size-1) = y0 - x;


    for (G4int i = 1; i< (n_points/8); i++){
      //We update x & y
      if (f > 0){
	y = y - 1;
        dy = dy + 2;
        f = f + dy;}
         
      x = x + 1;
      dx = dx + 2;
      f = f + dx;
         
      //1 octant
      xc.at(i) = x0 + x;
      yc.at(i) = y0 + y;
	 
      //2nd octant
      xc.at(8 * octant_size - i-1) = x0 - x;
      yc.at(8 * octant_size - i-1) = y0 + y;
         
      //3rd octant
      xc.at(4 * octant_size - i-1) = x0 + x;
      yc.at(4 * octant_size - i-1) = y0 - y;
         
      //4th octant
      xc.at(4 * octant_size + i) = x0 - x;
      yc.at(4 * octant_size + i) = y0 - y;
         
      //5th octant
      xc.at(2 * octant_size - i-1) = x0 + y;
      yc.at(2 * octant_size - i-1) = y0 + x;
      
      //6th octant
      xc.at(6 * octant_size + i) = x0 - y;
      yc.at(6 * octant_size + i) = y0 + x;
         
      //7th octant
      xc.at(2 * octant_size + i) = x0 + y;
      yc.at(2 * octant_size + i) = y0 - x;
         
      //8th octant
      xc.at(6 * octant_size - i-1) = x0 - y;
      yc.at(6 * octant_size - i-1) = y0 - x;
     
    }
         
//     G4cout << " " << endl;
//     for( vector<G4double>::const_iterator j = xc.begin(); j != xc.end(); ++j)
//     G4cout << *j << ' '; 
//     G4cout << " " << endl;
//     for( vector<G4double>::const_iterator j = yc.begin(); j != yc.end(); ++j)
//     G4cout << *j << ' '; 
//     G4cout << " " << endl;
    
    G4int copyN=1;
    G4int size=xc.size();

    for (G4int j = 0; j < size; j++) {
      if (j%octant_size==0)continue;
      new G4PVPlacement(0, G4ThreeVector(xc[j]*cm,yc[j]*cm,0.), logicVoxel, "Voxel", logicPhantom, false,copyN,checkOverlaps);
      copyN++;
    } 

    fScoringVolume=logicVoxel;    
    
    //Visualization attributes
    G4VisAttributes* worldVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0)); //White
    worldVisAtt->SetVisibility(true);
    fLogicWorld->SetVisAttributes(worldVisAtt);
    
    G4VisAttributes* phantomVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0)); //White
    phantomVisAtt->SetVisibility(true);
    logicPhantom->SetVisAttributes(phantomVisAtt);
    
    G4VisAttributes* outerShellVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,2.0)); //White
    outerShellVisAtt->SetVisibility(true);
    logicVoxel->SetVisAttributes(outerShellVisAtt);
    
    return fPhysicWorld;
}

 
