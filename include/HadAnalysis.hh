
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
class HadAnalysisMessenger;
//class ProdTuple_t;

class HadAnalysis {
private:
    static HadAnalysis* instance;

    G4String NtupleFileName;
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
    G4String material;
    Double_t enerPrimGen;
    G4String namePrim;
    G4ParticleDefinition* particle;
    G4String PartName;
    Int_t numberEvts;
    Int_t numberElastic;
    Int_t numberinElastic;
    Double_t totXs;
    Double_t elXs;
    Double_t inelXs;
    Double_t errtotXs;
    Double_t errelXs;
    Double_t errinelXs;
    HadAnalysisMessenger* anaMessenger;
public:

    HadAnalysis();
    ~HadAnalysis();

    void book();
    void FillNtuple(std::vector<TrackInfo_t>);

    void WriteNtuple();
    void WriteHeader();
    void SetDetConstInfo(std::vector<Double_t> DCinfo, G4Material* mat);
    void SetPrimGenInfo(Double_t enerPrim, G4ParticleDefinition* Part);
    void SetRunActInfo(Int_t nevt);
    void SetnumberElastic(Int_t nel);
    void SetnumberinElastic(Int_t ninel);
    void SetNtupleFilename(const G4String&);
    Int_t GetnumberElastic();
    Int_t GetnumberinElastic();

    G4String GetPartName() {
        return PartName;
    };

    G4String GetenerPrimGen() {
        return enerPrimGen;
    };

    G4String Getmaterial() {
        return material;
    };
    static HadAnalysis* getInstance();
    void finish();



};

#endif 
