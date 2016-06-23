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
#include "TPaveText.h"
#include "TAxis.h"

//This can be modified and used as a generic program to look at multiple .root
//files and put them all into one plot.

//Currently for lAr and proton, neutron

const int numberplots=2; // now it is 2 particles: proton, neutron

std::string directories[numberplots] = {"proton_lAr_QGSP_BERT","neutron_lAr_QGSP_BERT"};//change these directories to change particles/materials

std::string tuple_list1 = "tuplelist.txt";

const int max_datapoints = 300; 
//prepares arrays of information to gather from files
Float_t list_totXs[numberplots][max_datapoints];
Float_t list_errtotXs[numberplots][max_datapoints];
Float_t list_enerPrimGen[numberplots][max_datapoints];
Float_t list_elXs[numberplots][max_datapoints];
Float_t list_inelXs[numberplots][max_datapoints];
Float_t list_errelXs[numberplots][max_datapoints];
Float_t list_errinelXs[numberplots][max_datapoints];
Float_t zeros[max_datapoints]={0.0};
Int_t n_points[numberplots] = {0.0};

//plotnumber is 0,1,...,numberplots-1
void read_root_files(std::string filename, int plotnumber)
{
  //Open file, open Tree


  TFile *f = new TFile(("%s/%s" , directories[plotnumber], filename.c_str()), "OLD" );

  TTree *T2 = (TTree *) f->Get("RunInfo");
 //prepare variables to carry information from trees to the big arrays
  double enerPrimGen, totXs, errtotXs, elXs, inelXs, errinelXs, errelXs;


  // Access information from tree and store as variables
  T2->SetBranchAddress("totXs", &totXs);
  T2->SetBranchAddress("enerPrimGen", &enerPrimGen);
  T2->SetBranchAddress("errtotXs", &errtotXs);
  T2->SetBranchAddress("elXs", &elXs);
  T2->SetBranchAddress("inelXs", &inelXs);
  T2->SetBranchAddress("errelXs", &errelXs);
  T2->SetBranchAddress("errinelXs", &errinelXs);
  T2->GetEntry(0); // the only entry
  
 
  
  //put the variables into arrays to be graphed
  list_totXs[plotnumber][n_points[plotnumber]]=totXs;
  list_enerPrimGen[plotnumber][n_points[plotnumber]]=enerPrimGen;
  list_errtotXs[plotnumber][n_points[plotnumber]]=errtotXs;
  list_elXs[plotnumber][n_points[plotnumber]]=elXs;
  list_inelXs[plotnumber][n_points[plotnumber]]=inelXs;
  list_errelXs[plotnumber][n_points[plotnumber]]=errelXs;
  list_errinelXs[plotnumber][n_points[plotnumber]]=errinelXs;

  n_points[plotnumber]++;
}

void nucleons_lAr()
{
  //Prepare all of the graph things
  TCanvas *c1;//if you want two graphs, make a second canvas
  //and use c1->cd(); to change to canvas c1. Then you can draw different
  //plots to different canvases.
  TLegend *leg;
  TGraphErrors *gr_p_totXs; //p is for proton, n for neutron right now.
  TLegendEntry *le_p_totXs;
  TGraphErrors *gr_n_totXs;
  TLegendEntry *le_n_totXs;
  TGraphErrors *gr_p_elXs;
  TLegendEntry *le_p_elXs;
  TGraphErrors *gr_n_elXs;
  TLegendEntry *le_n_elXs;
  TGraphErrors *gr_p_inelXs;
  TLegendEntry *le_p_inelXs;
  TGraphErrors *gr_n_inelXs;
  TLegendEntry *le_n_inelXs;



  for (int j=0; j<numberplots; j++){
    // open list of .root files
    chdir(directories[j].c_str());
    ifstream  tp_list(tuple_list1.c_str());
    std::string tuple;


    //using the program above, fill up the arrays with values from the trees
    //it knows where to find the files from the tuple_list1
    while( tp_list >> tuple ) {
      read_root_files(tuple,j);
    }
    chdir("..");
  }
  
  c1 = new TCanvas("c1","XS Graph", 200,10,700,500);
  c1->SetGrid();


    
  // Put all the graphs on top of each other
  gr_p_totXs = new TGraphErrors(n_points[0],list_enerPrimGen[0],list_totXs[0],zeros,list_errtotXs[0]);
  gr_p_totXs -> SetMarkerColor(4);
  gr_p_totXs -> SetMarkerStyle(7);
  gr_p_totXs -> SetMinimum(0.0);
  // gr_pip_totXs -> Draw("LP");


  gr_n_totXs = new TGraphErrors(n_points[1],list_enerPrimGen[1],list_totXs[1],zeros,list_errtotXs[1]);
  gr_n_totXs -> SetTitle("Liquid Argon;Kinetic Energy [MeV];Total XS [mb]");
  gr_n_totXs -> SetMarkerColor(kBlue+3);
  gr_n_totXs -> SetMarkerStyle(7);
  gr_n_totXs -> SetMinimum(0.0);
  gr_n_totXs -> SetMaximum(2000);
  gr_n_totXs->GetXaxis()->SetLimits(0,2000);
  gr_n_totXs->GetYaxis()->SetTitleOffset(1.3);
  //I put the "A" here because this has the maximum
  gr_n_totXs -> Draw("ALP"); 
  gr_p_totXs -> Draw("LP");

  gr_p_elXs = new TGraphErrors(n_points[0],list_enerPrimGen[0],list_elXs[0],zeros,list_errelXs[0]);
  gr_p_elXs -> SetMarkerColor(kGreen);
  gr_p_elXs -> SetMarkerStyle(7);
  //gr_pip_elXs -> Draw("LP");


  gr_n_elXs = new TGraphErrors(n_points[1],list_enerPrimGen[1],list_elXs[1],zeros,list_errelXs[1]);
  gr_n_elXs -> SetMarkerColor(kGreen+3);
  gr_n_elXs -> SetMarkerStyle(7);
  gr_n_elXs -> Draw("LP");
  gr_p_elXs -> Draw("LP");

  gr_p_inelXs = new TGraphErrors(n_points[0],list_enerPrimGen[0],list_inelXs[0],zeros,list_errinelXs[0]);
  gr_p_inelXs -> SetMarkerColor(kRed);
  gr_p_inelXs -> SetMarkerStyle(7);
  //gr_pip_inelXs -> Draw("LP");


  gr_n_inelXs = new TGraphErrors(n_points[1],list_enerPrimGen[1],list_inelXs[1],zeros,list_errinelXs[1]);
  gr_n_inelXs -> SetMarkerColor(kRed+3);
  gr_n_inelXs -> SetMarkerStyle(7);
  gr_n_inelXs -> Draw("LP");
  gr_p_inelXs -> Draw("LP");


  //add a legend to the graph
  leg = new TLegend(0.7,0.9,0.9,0.7);


  le_n_totXs = leg-> AddEntry(gr_n_totXs,"Neutron Total XS","lep");
  le_p_totXs = leg-> AddEntry(gr_p_totXs,"Proton Total XS","lep");
  le_n_inelXs = leg-> AddEntry(gr_n_inelXs,"Neutron Inelastic XS","lep");
  le_p_inelXs = leg-> AddEntry(gr_p_inelXs,"Proton Inelastic XS","lep");
  le_n_elXs = leg-> AddEntry(gr_n_elXs,"Neutron Elastic XS","lep");
  le_p_elXs = leg-> AddEntry(gr_p_elXs,"Proton Elastic XS","lep");

  leg->Draw();

  //textbox
  TPaveText *pt = new TPaveText(0.4,0.85,0.65,0.75,"blNDC");
  pt->SetBorderSize(1);
  pt->SetFillColor(0);
  pt->SetTextAlign(12);
  pt->AddText("Geant 4.10.1.po2");
  pt->AddText("Physics List: QGSP_BERT");
  pt->AddText("Statistical Errors only");
  pt->Draw();
 
  
  c1 -> Update(); //not sure if this is necessary
 


  //Fermilab logo in corner

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
   c1->Print("nucleons_lAr_plots.png");

   
}



//And that's all, folks!   :)
