
#include "HadRunAction.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4Element.hh"
#include "HadAnalysis.hh"
#include "Randomize.hh"
//
#include "G4UnitsTable.hh"
#include "G4Neutron.hh"
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4NistManager.hh"
#include "G4HadronicProcessStore.hh"

#include "G4NucleiProperties.hh"
#include "G4NistManager.hh"
#include "G4StableIsotopes.hh"
#include "G4SystemOfUnits.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HadRunAction::HadRunAction() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HadRunAction::~HadRunAction() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HadRunAction::BeginOfRunAction(const G4Run* aRun) {
    G4int id = aRun->GetRunID();
    G4cout << "### Run " << id << " start" << G4endl;
    nEvts = aRun->GetNumberOfEventToBeProcessed();

    const G4long* table_entry;
    table_entry = CLHEP::HepRandom::getTheSeeds();
    G4long id0 = table_entry[0];
    G4long id1 = table_entry[1];

#ifdef G4VIS_USE
    G4UImanager* UI = G4UImanager::GetUIpointer();

    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();

    if (pVVisManager) {
        UI->ApplyCommand("/vis/scene/notifyHandlers");
    }
#endif


    HadAnalysis* analysis = HadAnalysis::getInstance();

    analysis->book(id0, id1);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HadRunAction::EndOfRunAction(const G4Run*) {
    G4cout << "RunAction: End of run actions are started" << G4endl;

#ifdef G4VIS_USE
    if (G4VVisManager::GetConcreteInstance())
        G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
#endif

    HadAnalysis* analysis = HadAnalysis::getInstance();
    analysis->GetRunActInfo(nEvts);

    analysis->finish();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
