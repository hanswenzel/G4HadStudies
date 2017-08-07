
#include "HadAnalysisMessenger.hh"

#include "HadAnalysis.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadAnalysisMessenger::HadAnalysisMessenger(HadAnalysis * ana)
:analysis(ana)
{
  anaDir = new G4UIdirectory("/testana/");
  anaDir->SetGuidance(" Hadronic Analysis.");


  fileCmd = new G4UIcmdWithAString("/testana/FileName",this);
  fileCmd->SetGuidance("Select Root output filename ");
  fileCmd->SetParameterName("NtupleFilename",false);
  fileCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadAnalysisMessenger::~HadAnalysisMessenger()
{
  delete fileCmd;
  delete anaDir;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadAnalysisMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
  if( command == fileCmd ) analysis->SetNtupleFilename(newValue);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

