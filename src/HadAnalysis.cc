
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <stdlib.h>
#include <math.h>

//Root 
#include <TFile.h>          
#include <TTree.h>

//GEANT4 
#include "globals.hh"
#include "G4ios.hh"
#include "G4Track.hh"
#include "G4SteppingManager.hh"
#include "G4ThreeVector.hh"
#include "G4TrajectoryContainer.hh"
#include "G4RunManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "G4Run.hh"
#include "G4Proton.hh"
#include "G4HadronicProcessStore.hh"
#include "G4NistManager.hh"

#include "HadAnalysis.hh"
#include "HadAnalysisMessenger.hh"

using namespace std;

HadAnalysis* HadAnalysis::instance = 0;

//------------------------------------------------------------------------------------

HadAnalysis::HadAnalysis() {
#ifdef G4ANALYSIS_USE
      anaMessenger = new HadAnalysisMessenger(this);
#endif
}
//------------------------------------------------------------------------------------

HadAnalysis::~HadAnalysis() {


#ifdef G4ANALYSIS_USE
#endif
}
//------------------------------------------------------------------------------------

HadAnalysis* HadAnalysis::getInstance() {
    if (instance == 0) instance = new HadAnalysis;
    return instance;
}
//------------------------------------------------------------------------------------

void HadAnalysis::book(G4long id0, G4long id1) {
//    sprintf(NtupleFileName, "ntuple_%04d.root", int(id0));

    G4cout << "Random Seeds: " << id0 << " " << id1 << G4endl;

    FileNtuple = new TFile(NtupleFileName.c_str(), "RECREATE", "hadronic interactions hadron-Target");

    //Get and store general information about this run: 
    HeaderTree = new TTree("RunInfo", "Run Info");
    HeaderTree->Branch("inelasticXS", &inelasticXS, "inelXS/D");
    HeaderTree->Branch("elasticXS", &elasticXS, "elXS/D");
    HeaderTree->Branch("thickness", &thickness, "thickness/D");
    HeaderTree->Branch("radius", &radius, "radius/D");
    HeaderTree->Branch("density", &rho, "density/D");
    HeaderTree->Branch("aweight", &aweight, "aweight/D");
    HeaderTree->Branch("material", &material);
    HeaderTree->Branch("enerPrimGen", &enerPrimGen, "enerPrimGen/D");
    HeaderTree->Branch("PartName", &PartName);
    HeaderTree->Branch("numberEvts", &numberEvts, "numberEvts/I");
    HeaderTree->Branch("numberElastic", &numberElastic, "numberElastic/I");
    HeaderTree->Branch("numberinElastic", &numberinElastic, "numberinElastic/I");
    HeaderTree->Branch("totXs", &totXs, "totXs/D");
    HeaderTree->Branch("elXs", &elXs, "elXs/D");
    HeaderTree->Branch("inelXs", &inelXs, "inelXs/D");
    HeaderTree->Branch("errtotXs", &errtotXs, "errtotXs/D");
    HeaderTree->Branch("errelXs", &errelXs, "errelXs/D");
    HeaderTree->Branch("errinelXs", &errinelXs, "errinelXs/D");
}


//------------------------------------------------------------------------------------

void HadAnalysis::finish() {


    FileNtuple->cd();
    //   ProdTree->Write();
    WriteHeader();
    FileNtuple->Close();
    //delete FileNtuple;
}

//------------------------------------------------------------------------------------

void HadAnalysis::FillNtuple(std::vector<TrackInfo_t> trackInfoVec) {
    g4Proddata.NPart = trackInfoVec.size();

    Int_t partNum = 0;

    std::vector<TrackInfo_t>::iterator iteTrackInfo = trackInfoVec.begin();
    for (; iteTrackInfo != trackInfoVec.end(); iteTrackInfo++) {

        g4Proddata.PDG[partNum] = (*iteTrackInfo).PDGcode;
        g4Proddata.InterType[partNum] = (*iteTrackInfo).interType;

        g4Proddata.X[partNum][0] = (*iteTrackInfo).Pos.X();
        g4Proddata.X[partNum][1] = (*iteTrackInfo).Pos.Y();
        g4Proddata.X[partNum][2] = (*iteTrackInfo).Pos.Z();

        g4Proddata.P[partNum][0] = (*iteTrackInfo).Mom.X();
        g4Proddata.P[partNum][1] = (*iteTrackInfo).Mom.Y();
        g4Proddata.P[partNum][2] = (*iteTrackInfo).Mom.Z();
        g4Proddata.P[partNum][3] = (*iteTrackInfo).Mom.E();

        partNum++;
    }

    if (g4Proddata.NPart > 0)WriteNtuple();
}

void HadAnalysis::WriteNtuple() {

    //    ProdTree->Fill();

}

void HadAnalysis::SetDetConstInfo(std::vector<Double_t> DCinfo, G4Material* mat) {
    thickness = DCinfo[0];
    radius = DCinfo[1];
    rho = DCinfo[2];
    mate = mat;
    material = mate->GetName();
}

void HadAnalysis::SetPrimGenInfo(Double_t enerPrim, G4ParticleDefinition* Part) {
    enerPrimGen = enerPrim;
    particle = Part;
    PartName = particle->GetParticleName();
}

void HadAnalysis::SetRunActInfo(Int_t nevt) {
    numberEvts = nevt;
}

void HadAnalysis::SetnumberElastic(Int_t nel) {
    numberElastic = nel;
}

void HadAnalysis::SetnumberinElastic(Int_t ninel) {
    numberinElastic = ninel;
}

Int_t HadAnalysis::GetnumberElastic() {
    return numberElastic;
}

Int_t HadAnalysis::GetnumberinElastic() {
    return numberinElastic;
}

void HadAnalysis::WriteHeader() {

    //XS:
    G4HadronicProcessStore* store = G4HadronicProcessStore::Instance();
    int pos = material.find("_");
    //G4cout << "Hello Material: "<< material << G4endl;
    G4String elemName = material.substr(pos + 1);
    // G4String elemName = "Ar";
    const G4Element* elm = G4NistManager::Instance()->FindOrBuildElement(elemName);
    aweight = elm->GetAtomicMassAmu();
    const double Na = 6.022140857e23;
    double convert = aweight / (Na * rho * thickness * 0.1);
    totXs = (1e27 * convert * (numberElastic + numberinElastic)) / numberEvts;
    elXs = (1e27 * convert * numberElastic) / numberEvts;
    inelXs = (1e27 * convert * numberinElastic) / numberEvts;
    errtotXs = totXs * sqrt(numberElastic + numberinElastic) / (numberElastic + numberinElastic);
    errelXs = elXs * sqrt(numberElastic) / numberElastic;
    errinelXs = inelXs * sqrt(numberinElastic) / numberinElastic;
    inelasticXS = 1.e25 * (store->GetInelasticCrossSectionPerAtom(particle, enerPrimGen, elm));
    elasticXS = 1.e25 * (store->GetElasticCrossSectionPerAtom(particle, enerPrimGen, elm));
    HeaderTree->Fill();
    HeaderTree->Write();

}
  void HadAnalysis::SetNtupleFilename(const G4String& fname)
{
      NtupleFileName=fname;
  }
