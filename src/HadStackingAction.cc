
#include "HadStackingAction.hh"
#include "HadAnalysis.hh"
#include "G4Track.hh"
#include "G4HadronicProcessStore.hh"
#include "G4NistManager.hh"
#include "HadEventAction.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadStackingAction::HadStackingAction()
{
  killSecondary  = false;
  pname          = ""; 
  elm            = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadStackingAction::~HadStackingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
HadStackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
 G4ClassificationOfNewTrack status = fUrgent;
 EvtManager = G4EventManager::GetEventManager();
 HadEvtAct = (HadEventAction*)(EvtManager -> GetUserEventAction());
 Bool_t procNP = false;
 Bool_t procPi = false;
 Bool_t procK  = false;

 if( (aTrack->GetTrackStatus() == fAlive) && (aTrack->GetParentID() == 1) ) {

   //if( aTrack->GetCreatorProcess()->GetProcessName() != "hIoni" ) {
   //     std::cout << "particle " << aTrack->GetDefinition()->GetParticleName() << " process " << aTrack->GetCreatorProcess()->GetProcessName() << std::endl;
   //}

   procNP = (aTrack->GetCreatorProcess()->GetProcessName()=="protonInelastic") || (aTrack->GetCreatorProcess()->GetProcessName()=="neutronInelastic");

   // make sure that it works with older versions of Geant4
   procPi = (aTrack->GetCreatorProcess()->GetProcessName()=="pi+Inelastic") || 
     (aTrack->GetCreatorProcess()->GetProcessName()=="PionPlusInelastic") ||
     (aTrack->GetCreatorProcess()->GetProcessName()=="pi-Inelastic") ||
     (aTrack->GetCreatorProcess()->GetProcessName()=="PionMinusInelastic");

   procK = (aTrack->GetCreatorProcess()->GetProcessName()=="kaon+Inelastic") || (aTrack->GetCreatorProcess()->GetProcessName()=="kaon-Inelastic");

   if(procNP || procPi || procK) HadEvtAct->AddTrack(aTrack,1);

   if(aTrack->GetCreatorProcess()->GetProcessName()=="hadElastic") HadEvtAct->AddTrack(aTrack,2);

 }
  //stack or delete secondaries
 if (killSecondary)      status = fKill;

 return status;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
