
#ifndef HadEventAction_h
#define HadEventAction_h 1
 
#include "G4UserEventAction.hh"
#include "globals.hh"
#include <vector>
#include "TrackInfo_t.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

class G4Event;
class G4Track;
class HadEventActionMessenger;
class G4UImanager;

class HadEventAction : public G4UserEventAction
{
public: // Without description

  HadEventAction();
  virtual ~HadEventAction();

  void BeginOfEventAction(const G4Event*);
  void   EndOfEventAction(const G4Event*);

  void SetPrintModulo(G4int val)   {printModulo = val;};
  void SetDrawFlag(G4String val)   {drawFlag = val;};
  void AddEventToDebug(G4int val)  {selectedEvents.push_back(val);
                                    nSelected++;};
  void AddTrack(const G4Track* aTrack,G4int proc);


private:

  HadEventActionMessenger* eventMessenger;
  G4UImanager*          UI;
  std::vector<G4int>    selectedEvents;

  G4int        printModulo;
  G4int        nSelected;

  // drawFlags = all, charged, neutral, charged+n
  G4String     drawFlag;
  G4bool       debugStarted;
  std::vector<TrackInfo_t> TrackInfoVec;
  
};

#endif


