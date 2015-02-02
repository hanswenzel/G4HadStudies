
#include "HadEventActionMessenger.hh"

#include "HadEventAction.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadEventActionMessenger::HadEventActionMessenger(HadEventAction* EvAct)
:eventAction(EvAct)
{ 
  drawCmd = new G4UIcmdWithAString("/testhadr/DrawTracks", this);
  drawCmd->SetGuidance("Draw the tracks in the event");
  drawCmd->SetGuidance("  Choice : neutral, charged, all");
  drawCmd->SetParameterName("choice",true);
  drawCmd->SetDefaultValue("all");
  drawCmd->SetCandidates("none charged all");
  drawCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
  
  printCmd = new G4UIcmdWithAnInteger("/testhadr/PrintModulo",this);
  printCmd->SetGuidance("Print events modulo n");
  printCmd->SetParameterName("EventNb",false);
  printCmd->SetRange("EventNb>0");
  printCmd->AvailableForStates(G4State_PreInit,G4State_Idle);      

  dCmd = new G4UIcmdWithAnInteger("/testhadr/DebugEvent",this);
  dCmd->SetGuidance("D event to debug");
  dCmd->SetParameterName("fNb",false);
  dCmd->SetRange("fNb>0");
  dCmd->AvailableForStates(G4State_PreInit,G4State_Idle);      

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadEventActionMessenger::~HadEventActionMessenger()
{
  delete drawCmd;
  delete printCmd;   
  delete dCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadEventActionMessenger::SetNewValue(G4UIcommand* command,
                                          G4String newValue)
{ 
  if(command == drawCmd)
    {eventAction->SetDrawFlag(newValue);}
    
  if(command == printCmd)
    {eventAction->SetPrintModulo(printCmd->GetNewIntValue(newValue));}           

  if(command == dCmd)
    {eventAction->AddEventToDebug(dCmd->GetNewIntValue(newValue));}           
   
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
