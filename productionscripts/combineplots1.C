#include <stdlib.h>
#include <TString.h>
#include <stdio.h>
#include <math.h>
#include <utility>
#include <string>
#include <iostream>
#include <fstream>
#include "TGraph.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TColor.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraphErrors.h"
#include "TROOT.h"
#include "TImage.h"
#include <vector>
#include <unistd.h>
#include "TPad.h"
#include "TText.h"
//Currently for lAr and pi+, pi-, kaon+, kaon-

const int numberplots=4; // now it is 4 particles: pi+, pi-, kaon+, kaon-

std::string directories[numberplots] = {"pi+_lAr_QGSP_BERT","pi-_lAr_QGSP_BERT","kaon+_lAr_QGSP_BERT","kaon-_lAr_QGSP_BERT"};

std::string tuple_list1 = "tuplelist.txt";

const int max_datapoints = 300; 

Float_t list_totXs[numberplots][max_datapoints];
Float_t list_errtotXs[numberplots][max_datapoints];
Float_t list_enerPrimGen[numberplots][max_datapoints];
Float_t zeros[max_datapoints]={0.0};

Int_t n_points[numberplots] = {0.0};

//plotnumber is 0,1,...,numberplots-1
void read_root_files(std::string filename, int plotnumber)
{
  //Open file, open Tree


  TFile *f = new TFile(("%s/%s" , directories[plotnumber], filename.c_str()), "OLD" );

  TTree *T2 = (TTree *) f->Get("RunInfo");
 
  double enerPrimGen, totXs, errtotXs;


  // Access information from tree and store as variables
  T2->SetBranchAddress("totXs", &totXs);
  T2->SetBranchAddress("enerPrimGen", &enerPrimGen);
  T2->SetBranchAddress("errtotXs", &errtotXs);
  T2->GetEntry(0); // the only entry
  
 
  
  //put the variables into arrays to be graphed
  list_totXs[plotnumber][n_points[plotnumber]]=totXs;
  list_enerPrimGen[plotnumber][n_points[plotnumber]]=enerPrimGen;
  list_errtotXs[plotnumber][n_points[plotnumber]]=errtotXs;

  n_points[plotnumber]++;
}

void combineplots1()
{
  //Prepare all of the graph things
  TCanvas *c1;
  TLegend *leg;
  TGraphErrors *gr_pip_totXs;
  TLegendEntry *le_pip_totXs;
  TGraphErrors *gr_pim_totXs;
  TLegendEntry *le_pim_totXs;
  TGraphErrors *gr_kaonp_totXs;
  TLegendEntry *le_kaonp_totXs;
  TGraphErrors *gr_kaonm_totXs;
  TLegendEntry *le_kaonm_totXs;



 

  for (int j=0; j<numberplots; j++){
    // open list of .root files
    chdir(directories[j].c_str());
    ifstream  tp_list(tuple_list1.c_str());
    std::string tuple;


    //using the program above, fill up the arrays with values from the trees
    while( tp_list >> tuple ) {
      read_root_files(tuple,j);
    }
    chdir("..");
  }
  
  c1 = new TCanvas("c1","XS Graph", 200,10,700,500);

  c1->SetGrid();


    
  // Put all the graphs on top of each other
  gr_pip_totXs = new TGraphErrors(n_points[0],list_enerPrimGen[0],list_totXs[0],zeros,list_errtotXs[0]);
  gr_pip_totXs -> SetTitle("Liquid Argon ;Kinetic Energy [MeV]; Total XS [mb]");
  gr_pip_totXs -> SetMarkerColor(4);
  gr_pip_totXs -> SetMarkerStyle(7);
  gr_pip_totXs -> SetMinimum(0.0);
  // gr_pip_totXs -> Draw("CP");


  gr_pim_totXs = new TGraphErrors(n_points[1],list_enerPrimGen[1],list_totXs[1],zeros,list_errtotXs[1]);
  gr_pim_totXs -> SetTitle("Liquid Argon ;Kinetic Energy [MeV]; Total XS [mb]");
  gr_pim_totXs -> SetMarkerColor(7);
  gr_pim_totXs -> SetMarkerStyle(7);
  gr_pim_totXs -> SetMinimum(0.0);
  gr_pim_totXs -> Draw("ACP"); //I put the "A" here because this has the maximum
  gr_pip_totXs -> Draw("CP");

  gr_kaonp_totXs = new TGraphErrors(n_points[2],list_enerPrimGen[2],list_totXs[2],zeros,list_errtotXs[2]);
  gr_kaonp_totXs -> SetMarkerColor(3);
  gr_kaonp_totXs -> SetMarkerStyle(7);
  gr_kaonp_totXs -> Draw("CP");

  gr_kaonm_totXs = new TGraphErrors(n_points[3],list_enerPrimGen[3],list_totXs[3],zeros,list_errtotXs[3]);
  gr_kaonm_totXs -> SetMarkerColor(2);
  gr_kaonm_totXs -> SetMarkerStyle(7);
  gr_kaonm_totXs -> Draw("CP");

  //add a legend to the graph
  leg = new TLegend(0.7,0.9,0.9,0.75);
  
  le_pip_totXs = leg-> AddEntry(gr_pip_totXs,"#pi^{+} Total XS","lep");
  le_pim_totXs = leg-> AddEntry(gr_pim_totXs,"#pi^{-} Total XS","lep");
  le_kaonp_totXs = leg-> AddEntry(gr_kaonp_totXs,"K^{+} Total XS","lep");
  le_kaonm_totXs = leg-> AddEntry(gr_kaonm_totXs,"K^{-} Total XS","lep");
  leg->Draw();



  //Fermilab logo

   TImage *img = TImage::Open("fermi_logo.png");

   if (!img) {
      printf("Could not create an image... exit\n");
      return;
   }
   

   TPad *l = new TPad("l","l",.9,.9,1,1);
   //gPad->cd(0);
   l->Draw();
   l->cd();
   img->Draw();



   // put it all onto a file
   c1->Print("combineplots1_plots.png");

   
}



//And that's all, folks!   :)

