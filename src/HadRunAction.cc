
#include "HadRunAction.hh"
#include "G4UImanager.hh"
#include "G4VVisManager.hh"
#include "G4Element.hh"
#include "HadAnalysis.hh"
#include "HadPrimaryGeneratorAction.hh"
#include "Randomize.hh"
//
#include "G4UnitsTable.hh"
#include "G4Neutron.hh"
#include "globals.hh"
#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleGun.hh"
#include "G4NistManager.hh"
#include "G4HadronicProcessStore.hh"

#include "G4NucleiProperties.hh"
#include "G4NistManager.hh"
#include "G4StableIsotopes.hh"
#include "G4SystemOfUnits.hh"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HadRunAction::HadRunAction() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

HadRunAction::~HadRunAction() {
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HadRunAction::BeginOfRunAction(const G4Run* aRun) {
    G4int id = aRun->GetRunID();
    G4cout << "### Run " << id << " start" << endl;
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

    analysis->book();
    analysis->SetnumberElastic(0);
    analysis->SetnumberinElastic(0);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void HadRunAction::EndOfRunAction(const G4Run*) {
    G4cout << "RunAction: End of run actions are started" << endl;

#ifdef G4VIS_USE
    if (G4VVisManager::GetConcreteInstance())
        G4UImanager::GetUIpointer()->ApplyCommand("/vis/viewer/update");
#endif

    HadAnalysis* analysis = HadAnalysis::getInstance();
    analysis->SetRunActInfo(nEvts);
    analysis->finish();

//    G4String fElementName = "H";
//    G4String fParticleName = "pi-";
    string filename = "XS_"+analysis->GetPartName()+"_"+analysis->Getmaterial()+".out";

    ofstream myfile;
    
    myfile.open (filename);
    myfile << "-------------------------------" << analysis->Getmaterial() << endl;
    myfile << "-------------------------------" << analysis->GetPartName() << endl;
    G4String fElementName  = analysis->Getmaterial().substr(3);
    G4String fParticleName = analysis->GetPartName() ;
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
            G4NistManager::Instance()->FindOrBuildMaterial(analysis->Getmaterial());
    const G4ParticleDefinition* particle =
            G4ParticleTable::GetParticleTable()->FindParticle(analysis->GetPartName());

    myfile << "### Fill Cross Sections for " << fParticleName
            << " off " << fElementName
            << endl;
    myfile << "-------------------------------------------------------------"
            << endl;
    myfile << "    N     E(MeV)   Elastic(b)   Inelastic(b)";
    myfile << "   Total(b)" << endl;
    myfile << "-------------------------------------------------------------"
            << endl;
    if (!particle || !elm) {
        G4cout << "HistoManager WARNING Particle or element undefined" << endl;
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
        myfile <<setprecision(5) << std::setw(5) << i << scientific<< std::setw(12) << e;
        xs = store->GetElasticCrossSectionPerAtom(particle, e, elm, mat);
        xtot = xs;
        myfile  <<fixed<<setprecision(5)<<std::setw(12) << xs / barn;
        xs = store->GetInelasticCrossSectionPerAtom(particle, e, elm, mat);
        myfile <<fixed<<setprecision(5)<< std::setw(12) << xs / barn;
        xtot += xs;
        myfile << " " << std::setw(12) << xtot / barn << endl;
        if (fTargetMaterial) {
            xs =
                    store->GetInelasticCrossSectionPerVolume(particle, e, fTargetMaterial);
            xs =
                    store->GetElasticCrossSectionPerVolume(particle, e, fTargetMaterial);
        }

    }
    myfile << "-------------------------------------------------------------"
            << endl;
    myfile.close();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....
