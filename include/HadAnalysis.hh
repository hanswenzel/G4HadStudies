
#ifndef HadANALYSIS_HH
#define HadANALYSIS_HH

#include "globals.hh"

//root
#include "TSystem.h"

//G4
#include "G4ios.hh"
#include "G4TrajectoryContainer.hh"
#include "G4ParticleDefinition.hh"
#include "G4Element.hh"
#include "G4Material.hh"

#include <map>
#include <vector>

#include "ProdTuple_t.hh"
#include "TrackInfo_t.hh"

class G4ParticleDefinition;
class G4Step;
class TFile;
class TTree;
class G4Track;
class G4VTrajectory;

//class ProdTuple_t;

class HadAnalysis {
public:

    HadAnalysis();
    ~HadAnalysis();

    void book(G4long, G4long);
    void FillNtuple(std::vector<TrackInfo_t>);

    void WriteNtuple();
    void WriteHeader();
    void SetDetConstInfo(std::vector<Double_t> DCinfo, G4Material* mat);
    void SetPrimGenInfo(Double_t enerPrim, G4ParticleDefinition* Part);
    void SetRunActInfo(Int_t nevt);
    void SetnumberElastic(Int_t nel);
    void SetnumberinElastic(Int_t ninel);
    Int_t GetnumberElastic();
    Int_t GetnumberinElastic();
    static HadAnalysis* getInstance();
    void finish();


private:
    static HadAnalysis* instance;

    char NtupleFileName[50];
    TFile* FileNtuple;
    //   TTree* ProdTree;
    TTree* HeaderTree;
    ProdTuple_t g4Proddata;
    //info for header:
    Double_t inelasticXS;
    Double_t elasticXS;
    Double_t thickness;
    Double_t radius;
    Double_t rho;
    Double_t aweight;
    G4Material* mate;
    std::string material;
    Double_t enerPrimGen;
    std::string namePrim;
    G4ParticleDefinition* particle;
    std::string PartName;
    Int_t numberEvts;
    Int_t numberElastic;
    Int_t numberinElastic;
    Double_t totXs;
    Double_t elXs;
    Double_t inelXs;
    Double_t errtotXs;
    Double_t errelXs;
    Double_t errinelXs;
};

#endif 
