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
/// \file include/PSIG1TrackInformation.hh
/// \brief Definition of the PSIG1TrackInformation class
//
//
// $Id$
//

#ifndef TrackInformation_h
#   define TrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"
#include "G4Types.hh"

class     TrackInformation:public G4VUserTrackInformation {
  public:
    TrackInformation();
    TrackInformation(const G4Track * aTrack);
    TrackInformation(const TrackInformation * aTrackInfo);
    virtual ~ TrackInformation();

    inline void *operator  new(size_t);
    inline void operator  delete(void *aTrackInfo);
    inline int operator ==(const TrackInformation & right) const {
        return (this == &right);
    } void    Print() const;

  private:
    G4int originalTrackID;
    G4int     parentTrackID;
    G4String  particleName;
    G4ParticleDefinition *particleDefinition;
    G4ThreeVector originalPosition;
    G4ThreeVector originalMomentum;
    G4double  originalEnergy;
    G4double  originalTime;

  public:
              inline G4int GetOriginalTrackID() const {
        return originalTrackID;
    } inline G4int GetParentTrackID() const {
        return parentTrackID;
    } inline G4String GetTriggerParticleName() const {
        return particleName;
    } inline G4ParticleDefinition *GetOriginalParticle() const {
        return particleDefinition;
    } inline G4ThreeVector GetOriginalPosition() const {
        return originalPosition;
    } inline G4ThreeVector GetOriginalMomentum() const {
        return originalMomentum;
    } inline G4double GetOriginalEnergy() const {
        return originalEnergy;
    } inline G4double GetOriginalTime() const {
        return originalTime;
}};

extern  G4ThreadLocal  G4Allocator < TrackInformation >* aTrackInformationAllocator;

inline void *TrackInformation::operator  new(size_t)
{
    if(!aTrackInformationAllocator)  aTrackInformationAllocator = new G4Allocator<TrackInformation>;
    void     *aTrackInfo;
    aTrackInfo = (void *) aTrackInformationAllocator->MallocSingle();
    return aTrackInfo;
}

inline void TrackInformation::operator  delete(void *aTrackInfo)
{
    aTrackInformationAllocator->FreeSingle((TrackInformation *) aTrackInfo);
}

#endif
