
#ifndef HadEventActionMessenger_h
#define HadEventActionMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class HadEventAction;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HadEventActionMessenger: public G4UImessenger
{
public:

  HadEventActionMessenger(HadEventAction*);
  virtual ~HadEventActionMessenger();
    
  void SetNewValue(G4UIcommand*, G4String);
    
private:

  HadEventAction*          eventAction;   
  G4UIcmdWithAString*   drawCmd;
  G4UIcmdWithAnInteger* printCmd;    
  G4UIcmdWithAnInteger* dCmd;    

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
