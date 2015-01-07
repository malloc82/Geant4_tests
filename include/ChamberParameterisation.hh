#ifndef ChamberParameterisation_h
#define ChamberParameterisation_h 1

#include "globals.hh"
#include "G4VPVParameterisation.hh"
#include "G4RotationMatrix.hh"

class G4VPhysicalVolume;
class G4Tubs;

// Dummy declarations to get rid of warnings ...
class G4Trd;
class G4Trap;
class G4Cons;
class G4Orb;
class G4Sphere;
class G4Torus;
class G4Para;
class G4Hype;
class G4Box;
class G4Polycone;
class G4Polyhedra;


class ChamberParameterisation : public G4VPVParameterisation

{
public:

  ChamberParameterisation(G4int NoChambers, 
			  G4double startZ,
			  G4double startY,
			  G4double spacingZ, 
			  G4double radius,
			  G4double high,
			  G4double zDisp
 			);
  
virtual 
  ~ChamberParameterisation();
  
  void ComputeTransformation (const G4int copyNo,
			      G4VPhysicalVolume* physVol) const;
			      
  void ComputeDimensions (G4Tubs& trackerLayer, 
			  const G4int copyNo,
			  const G4VPhysicalVolume* physVol) const;
			  
private:  // Dummy declarations to get rid of warnings ...

  void ComputeDimensions (G4Box&, const G4int, const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Trd&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Trap&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Cons&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Sphere&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Orb&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Torus&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Para&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Hype&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Polycone&,const G4int,const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Polyhedra&,const G4int,const G4VPhysicalVolume*) const {}
   
   G4int _NoChambers;
   G4double _startX;
   G4double _startY;
   G4double _spacing; 
   G4double _radius;
   G4double _height;
   G4RotationMatrix* _yRot90deg;
   G4double _zDisplacement;
    
//    G4RotationMatrix* yRot90deg;
			  
};

#endif