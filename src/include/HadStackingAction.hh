
#ifndef HadStackingAction_h
#define HadStackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"
#include "G4ParticleDefinition.hh"
#include "HadAnalysis.hh"

class G4Track;
class G4EventManager;
class HadEventAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HadStackingAction : public G4UserStackingAction
{
public:

  HadStackingAction();
  virtual ~HadStackingAction();
   
  void SetKillStatus(G4bool value)    {killSecondary = value;};
  void SetKill(const G4String& name)  {pname = name;};
     
  G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);
    
private:


  G4String                    pname;
  G4bool                      killSecondary;
  const G4ParticleDefinition* primaryDef;
  G4double                    primaryTotalEnergy;
  G4Element*                  elm;
  G4EventManager *EvtManager;
  HadEventAction *HadEvtAct;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

