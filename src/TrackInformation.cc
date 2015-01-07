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
/// \file src/PSIG1TrackInformation.cc
/// \brief Implementation of the PSIG1TrackInformation class
//
//
// $Id$
//

#include "TrackInformation.hh"

#include "G4ios.hh"

G4ThreadLocal G4Allocator <TrackInformation>* aTrackInformationAllocator=0;

TrackInformation::TrackInformation()
{
    originalTrackID = 0;
    parentTrackID = 0;
    particleName = 0;
    particleDefinition = 0;
    originalPosition = G4ThreeVector(0., 0., 0.);
    originalMomentum = G4ThreeVector(0., 0., 0.);
    originalEnergy = 0.;
    originalTime = 0.;
}

TrackInformation::TrackInformation(const G4Track* aTrack)
{
    originalTrackID = aTrack->GetTrackID();
    parentTrackID = aTrack->GetParentID();
    particleName = aTrack->GetDynamicParticle()->GetDefinition()->GetParticleName();
    particleDefinition = aTrack->GetDefinition();
    originalPosition = aTrack->GetPosition();
    originalMomentum = aTrack->GetMomentum();
    originalEnergy = aTrack->GetTotalEnergy();
    originalTime = aTrack->GetGlobalTime();
}

TrackInformation::TrackInformation(const TrackInformation*
        aTrackInfo)
{
    originalTrackID = aTrackInfo->originalTrackID;
    parentTrackID = aTrackInfo->parentTrackID;
    particleName = aTrackInfo->particleName;
    particleDefinition = aTrackInfo->particleDefinition;
    originalPosition = aTrackInfo->originalPosition;
    originalMomentum = aTrackInfo->originalMomentum;
    originalEnergy = aTrackInfo->originalEnergy;
    originalTime = aTrackInfo->originalTime;
}

TrackInformation::~TrackInformation()
{}

void TrackInformation::Print() const
{
//     G4cout
//             << "Original track ID " << originalTrackID
//             << " at " << originalPosition << G4endl;
}
