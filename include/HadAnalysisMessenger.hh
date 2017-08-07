#ifndef HadAnalysisMessenger_h
#define HadAnalysisMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class HadAnalysis;
class G4UIdirectory;
class G4UIcmdWithAString;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class HadAnalysisMessenger: public G4UImessenger
{
public:

  HadAnalysisMessenger(HadAnalysis* );
  virtual ~HadAnalysisMessenger();

  void SetNewValue(G4UIcommand*, G4String);

private:

  HadAnalysis* analysis;

  G4UIdirectory*             anaDir;
  G4UIcmdWithAString*        fileCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

