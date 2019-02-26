#include "HadSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4VProcess.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "HadAnalysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadSD::HadSD(const G4String& name,
        const G4String& hitsCollectionName)
: G4VSensitiveDetector(name) {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HadSD::~HadSD() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadSD::Initialize(G4HCofThisEvent* hce) {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool HadSD::ProcessHits(G4Step* aStep,
        G4TouchableHistory*) {
    //
    // we only care about the first elastic or inelastic interaction of the primary particle
    // (geant4 version 4.10 and up) )
    //  
    if (aStep->GetTrack()->GetParentID() == 0) {
      std::cout<< aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName()<<std::endl;
        if (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "hadElastic") {
            HadAnalysis* analysis = HadAnalysis::getInstance();
            analysis->SetnumberElastic((analysis->GetnumberElastic()) + 1);
            return true;
        }
        if ((aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "pi-Inelastic") ||
                (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "pi+Inelastic") ||
                (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "protonInelastic") ||
                (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "neutronInelastic") ||
                (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "kaon-Inelastic") ||
                (aStep->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() == "kaon+Inelastic")) {

            HadAnalysis* analysis = HadAnalysis::getInstance();
            analysis->SetnumberinElastic((analysis->GetnumberinElastic()) + 1);
            return true;
        }
    }
    return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HadSD::EndOfEvent(G4HCofThisEvent*) {

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
