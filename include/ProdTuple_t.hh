
#ifndef PRODTUPLE_T_HH
#define PRODTUPLE_T_HH

#include "TROOT.h"        
#include "TObject.h"
#include "Rtypes.h"

const Int_t maxPart = 200;


class ProdTuple_t 
{
  
 public:
  // a constructor and a destructor
  ProdTuple_t();
  virtual ~ProdTuple_t();
  
  // the following variables are placed in the root tree
  Int_t NPart;
  Int_t PDG[maxPart];
  Double_t X[maxPart][3];//Initial position of the track
  Double_t P[maxPart][4];//4-momentum. P[maxPart][3] is the energy
  Int_t InterType[maxPart]; //1: Hadronic Inelastic,   2: Hadronic Elastic
 
 private:
   ClassDef(ProdTuple_t , 1)
    
    };
#endif 

