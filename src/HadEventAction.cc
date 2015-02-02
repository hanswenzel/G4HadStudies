#include "HadEventAction.hh"
#include "G4Event.hh"
#include "HadEventActionMessenger.hh"
#include "HadAnalysis.hh"

#include "G4UImanager.hh"
#include "G4ios.hh"
#include "G4Track.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HadEventAction::HadEventAction():
  printModulo(100),
  nSelected(0),
  drawFlag("all"),
  debugStarted(false)
{
  eventMessenger = new HadEventActionMessenger(this);
  UI = G4UImanager::GetUIpointer();
  selectedEvents.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HadEventAction::~HadEventAction()
{
  delete eventMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HadEventAction::BeginOfEventAction(const G4Event* evt)
{
  // New event
  G4int nEvt = evt->GetEventID();
  if(nEvt%100000==0)G4cout<<"EventID " <<nEvt<<G4endl;
 if(nSelected>0) {
    for(G4int i=0; i<nSelected; i++) {
      if(nEvt == selectedEvents[i]) {
        UI->ApplyCommand("/random/saveThisEvent");
        UI->ApplyCommand("/tracking/verbose  2");
        debugStarted = true;
        break;
      }
    }
  }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HadEventAction::AddTrack(const G4Track* aTrack,G4int proc)
{
  
  TrackInfo_t aTrackInfo;
  aTrackInfo.PDGcode = (aTrack->GetDefinition())->GetPDGEncoding();
  aTrackInfo.massPart= (aTrack->GetDefinition())->GetPDGMass();
  aTrackInfo.Pos.SetX(aTrack->GetPosition().x());
  aTrackInfo.Pos.SetY(aTrack->GetPosition().y());
  aTrackInfo.Pos.SetZ(aTrack->GetPosition().z());
  aTrackInfo.Mom.SetPx(aTrack->GetMomentum().x());
  aTrackInfo.Mom.SetPy(aTrack->GetMomentum().y());
  aTrackInfo.Mom.SetPz(aTrack->GetMomentum().z());
  aTrackInfo.Mom.SetE(aTrack->GetTotalEnergy());
  if(proc==1)aTrackInfo.interType = 1; 
  if(proc==2)aTrackInfo.interType = 2;
  if(proc!=1 && proc!=2)aTrackInfo.interType = 0;

  TrackInfoVec.push_back(aTrackInfo);
 
}


void HadEventAction::EndOfEventAction(const G4Event* evt)
{
  
  if(debugStarted) {
    G4cout<<"End of debug: eventID: "<<(evt->GetEventID())<<G4endl;
    UI->ApplyCommand("/tracking/verbose  0");
    debugStarted = false;
  }

  HadAnalysis* analysis = HadAnalysis::getInstance();
  if(TrackInfoVec.size()<100)analysis->FillNtuple(TrackInfoVec);
  TrackInfoVec.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
