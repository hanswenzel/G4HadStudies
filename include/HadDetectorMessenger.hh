
#ifndef HadDetectorMessenger_h
#define HadDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class HadDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithoutParameter;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HadDetectorMessenger: public G4UImessenger
{
public:

  HadDetectorMessenger(HadDetectorConstruction* );
  virtual ~HadDetectorMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:

  HadDetectorConstruction* Detector;

  G4UIdirectory*             testDir;
  G4UIcmdWithAString*        matCmd;
  G4UIcmdWithAString*        mat1Cmd;
  G4UIcmdWithADoubleAndUnit* rCmd;
  G4UIcmdWithADoubleAndUnit* lCmd;
  G4UIcmdWithADoubleAndUnit* edepCmd;
  G4UIcmdWithAnInteger*      binCmd;
  G4UIcmdWithAnInteger*      nOfAbsCmd;
  G4UIcmdWithAnInteger*      verbCmd;
  G4UIcmdWithABool*          beamCmd;
  G4UIcmdWithoutParameter*   updateCmd;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

