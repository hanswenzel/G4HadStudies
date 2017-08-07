
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
    analysis->SetnumberElastic(0);
    analysis->SetnumberinElastic(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HadRunAction::EndOfRunAction(const G4Run*) {
    G4cout << "RunAction: End of run actions are started" << G4endl;

#ifdef G4VIS_USE
    if (G4VVisManager::GetConcreteInstance())
        G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
#endif

    HadAnalysis* analysis = HadAnalysis::getInstance();
    analysis->SetRunActInfo(nEvts);
    analysis->finish();
    G4String fElementName = "H";
    G4String fParticleName = "pi-";
    G4double fMinKinEnergy;
    G4double fMaxKinEnergy;
    G4double fMinMomentum;
    G4double fMaxMomentum;

    G4int fVerbose;
    G4int fBinsE;
    G4int fBinsP;
    fMinKinEnergy = 0.1 * MeV;
    fMaxKinEnergy = 10 * TeV;
    fMinMomentum = 1 * MeV;
    fMaxMomentum = 10 * TeV;

    fBinsE = 800;
    fBinsP = 700;
    const G4Element* elm =
            G4NistManager::Instance()->FindOrBuildElement(fElementName);
    const G4Material* mat =
            G4NistManager::Instance()->FindOrBuildMaterial("G4_" + fElementName);
    const G4ParticleDefinition* particle =
            G4ParticleTable::GetParticleTable()->FindParticle(fParticleName);

    G4cout << "### Fill Cross Sections for " << fParticleName
            << " off " << fElementName
            << G4endl;
    G4cout << "-------------------------------------------------------------"
            << G4endl;
    G4cout << "    N     E(MeV)   Elastic(b)   Inelastic(b)";
    G4cout << "   Total(b)" << G4endl;
    G4cout << "-------------------------------------------------------------"
            << G4endl;
    if (!particle || !elm) {
        G4cout << "HistoManager WARNING Particle or element undefined" << G4endl;
        return;
    }

    G4int prec = G4cout.precision();
    G4cout.precision(4);

    G4HadronicProcessStore* store = G4HadronicProcessStore::Instance();
    G4double mass = particle->GetPDGMass();

    // Build histograms

    G4double p1 = std::log10(fMinMomentum / GeV);
    G4double p2 = std::log10(fMaxMomentum / GeV);
    G4double e1 = std::log10(fMinKinEnergy / MeV);
    G4double e2 = std::log10(fMaxKinEnergy / MeV);
    G4double de = (e2 - e1) / G4double(fBinsE);
    G4double dp = (p2 - p1) / G4double(fBinsP);

    G4double x = e1 - de * 0.5;
    G4double e, p, xs, xtot;
    G4int i;
    const G4Material* fTargetMaterial;
    fTargetMaterial = G4NistManager::Instance()->FindOrBuildMaterial("G4_H");
    G4double coeff = 1.0;
    if (fTargetMaterial) {
        coeff = fTargetMaterial->GetDensity() * cm2 / g;
    }

    for (i = 0; i < fBinsE; i++) {
        x += de;
        e = std::pow(10., x) * MeV;
        G4cout << std::setw(5) << i << std::setw(12) << e;
        xs = store->GetElasticCrossSectionPerAtom(particle, e, elm, mat);
        xtot = xs;
        G4cout << std::setw(12) << xs / barn;
        xs = store->GetInelasticCrossSectionPerAtom(particle, e, elm, mat);
         G4cout << std::setw(12) << xs / barn;
        xtot += xs;
        G4cout << " " << std::setw(12) << xtot/barn << G4endl;  
        if (fTargetMaterial) {
            xs =
                    store->GetInelasticCrossSectionPerVolume(particle, e, fTargetMaterial);
            xs =
                    store->GetElasticCrossSectionPerVolume(particle, e, fTargetMaterial);
        }

    }
  G4cout << "-------------------------------------------------------------" 
           << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
