
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "Randomize.hh"
#include "G4PhysListFactory.hh"
#include "G4OpticalPhysics.hh"
#include "G4VModularPhysicsList.hh"

#include "HadDetectorConstruction.hh"
#include "HadPrimaryGeneratorAction.hh"

#include "HadRunAction.hh"
#include "HadEventAction.hh"
#include "HadStackingAction.hh"

#include "G4VisExecutive.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) {

  //choose the Random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());

  //Construct the default run manager
  G4RunManager * runManager = new G4RunManager();
    G4PhysListFactory factory;
    G4VModularPhysicsList* phys = NULL;
    G4String physName = "";
    //-----------------------------------------------------
    // Physics List name defined via environmental variable
    // The following 19 physics lists are available:
    //  CHIPS
    //  FTFP_BERT
    //  FTFP_BERT_TRV
    //  FTFP_BERT_HP
    //  FTF_BIC 
    //  LBE
    //  LHEP
    //  QBBC
    //  QGSC_BERT
    //  QGSP
    //  QGSP_BERT
    //  QGSP_BERT_CHIPS
    //  QGSP_BERT_HP
    //  QGSP_BIC
    //  QGSP_BIC_HP
    //  QGSP_FTFP_BERT
    //  QGS_BIC
    //  QGSP_INCLXX
    //  Shielding
    //-----------------------------------------------------
    char* path = getenv("PHYSLIST");
    if (path) {
        physName = G4String(path);
    } else {
        physName = "FTFP_BERT"; // default
    }
    // reference PhysicsList via its name
    if (factory.IsReferencePhysList(physName)) {
        phys = factory.GetReferencePhysList(physName);
    }
  //set mandatory initialization classes
  runManager->SetUserInitialization(new HadDetectorConstruction());

    runManager->SetUserInitialization(phys);
  //runManager->SetUserInitialization(new QBBC(1,"QBEC_HP"));
  //runManager->SetUserInitialization(new QGSP);

  runManager->SetUserAction(new HadPrimaryGeneratorAction());

  //set user action classes
  runManager->SetUserAction(new HadRunAction());
  runManager->SetUserAction(new HadEventAction());
  runManager->SetUserAction(new HadStackingAction());

  //get the pointer to the User Interface manager
  G4UImanager* UI = G4UImanager::GetUIpointer();
  G4VisManager* visManager = 0;

  if (argc==1)   // Define UI terminal for interactive mode
    {
#ifdef G4VIS_USE
      //visualization manager
      visManager = new G4VisExecutive;
      visManager->Initialize();
#endif
      G4UIsession* session = 0;
#ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);
#else
      session = new G4UIterminal();
#endif
      session->SessionStart();
      delete session;
    }
  else           // Batch mode
    {
     G4String command = "/control/execute ";
     G4String fileName = argv[1];
     UI->ApplyCommand(command+fileName);
    }

  //job termination
  if(visManager) delete visManager;
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
