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
#include "TAxis.h"
#include <vector>

//get location of .root files
std::string tuple_list = "tuplelist.txt";

const int max_datapoints = 300; //Change this number if you want 300 or more bins

//prepare arrays of datapoints
Float_t list_totXs[max_datapoints];
Float_t list_elXs[max_datapoints];
Float_t list_inelXs[max_datapoints];
Float_t list_errtotXs[max_datapoints];
Float_t list_errelXs[max_datapoints];
Float_t list_errinelXs[max_datapoints];
Float_t list_enerPrimGen[max_datapoints];
Float_t zeros[max_datapoints]={};


//these are the G4 values pulled from some chart
Float_t list_inelasticXS[max_datapoints];
Float_t list_elasticXS[max_datapoints];
Float_t list_totalXS[max_datapoints];


Int_t n_points = 0;

void read_root_files( std::string filename)
{

  //Open file, open Tree
  TFile *f = new TFile( filename.c_str(), "OLD" );
  TTree *T2 = (TTree *) f->Get("RunInfo");

  double elXs, inelXs,  enerPrimGen, totXs, errtotXs, errelXs, errinelXs, elasticXS, inelasticXS;


  // Access information from tree and store as variables
  T2->SetBranchAddress("elXs", &elXs);
  T2->SetBranchAddress("inelXs", &inelXs);
  T2->SetBranchAddress("totXs", &totXs);
  T2->SetBranchAddress("enerPrimGen", &enerPrimGen);
  T2->SetBranchAddress("errelXs", &errelXs);
  T2->SetBranchAddress("errinelXs", &errinelXs);
  T2->SetBranchAddress("errtotXs", &errtotXs);
  T2->SetBranchAddress("elasticXS", &elasticXS);
  T2->SetBranchAddress("inelasticXS", &inelasticXS);
  T2->GetEntry(0); // the only entry
  
  n_points++;
  //put the variables into arrays to be graphed
  list_totXs[n_points-1]=totXs;
  list_elXs[n_points-1]=elXs;
  list_inelXs[n_points-1]=inelXs;
  list_enerPrimGen[n_points-1]=enerPrimGen;
  list_errtotXs[n_points-1]=errtotXs;
  list_errelXs[n_points-1]=errelXs;
  list_errinelXs[n_points-1]=errinelXs;
  list_elasticXS[n_points-1]=elasticXS;
  list_inelasticXS[n_points-1]=inelasticXS;
  list_totalXS[n_points-1]=elasticXS + inelasticXS;
}

void readtuple2()
{
  //Prepare all of the graph things
  TCanvas *c1;
  TGraphErrors *gr_totXs;
  TGraphErrors *gr_elXs;
  TGraphErrors *gr_inelXs;
  TLegend *leg;
  TLegendEntry *le_totXs;
  TLegendEntry *le_elXs;
  TLegendEntry *le_inelXs;
  TLegendEntry *le_totalXS;
  TLegendEntry *le_elasticXS;
  TLegendEntry *le_inelasticXS;
  TGraph *gr_totalXS;
  TGraph *gr_elasticXS;
  TGraph *gr_inelasticXS;

  // open up the list of .root files
  ifstream  tp_list(tuple_list.c_str());
  std::string tuple;

  //using the program above, fill up the arrays with values from the trees
  while( tp_list >> tuple ) {
    read_root_files(tuple);
  }


  // Start Canvas?
  c1 = new TCanvas("c1","XS Graph", 200,10,700,500);
  c1->SetGrid();
  
    
  // Put all the graphs on top of each other
  gr_totXs = new TGraphErrors(n_points,list_enerPrimGen,list_totXs,zeros,list_errtotXs);
  gr_totXs -> SetTitle("XS vs Energy");
  gr_totXs -> SetMarkerColor(4);
  gr_totXs -> SetMarkerStyle(7);
  gr_totXs -> SetMinimum(0.0);
  
  //Unforunately the axis Labels keep on giving me errors
  //if you want them, I think you just have to #include "TAxis.h"
  gr_totXs -> GetXaxis() -> SetTitle("Energy [Mev]");
  gr_totXs -> GetYaxis() -> SetTitle("XS [mb]");
  gr_totXs -> Draw("ALP");
  
  gr_elXs = new TGraphErrors(n_points,list_enerPrimGen,list_elXs,zeros,list_errelXs);
  gr_elXs -> SetMarkerColor(3);
  gr_elXs -> SetMarkerStyle(7);
  gr_elXs -> Draw("LP");

  gr_inelXs = new TGraphErrors(n_points,list_enerPrimGen,list_inelXs,zeros,list_errinelXs);
  gr_inelXs -> SetMarkerColor(2);
  gr_inelXs -> SetMarkerStyle(7);
  gr_inelXs -> Draw("LP");

  gr_totalXS = new TGraph(n_points, list_enerPrimGen,list_totalXS);
  gr_totalXS -> SetLineColor(4);
  gr_totalXS -> Draw("LP");

  gr_elasticXS = new TGraph(n_points, list_enerPrimGen,list_elasticXS);
  gr_elasticXS -> SetLineColor(3);
  gr_elasticXS -> Draw("LP");

  gr_inelasticXS = new TGraph(n_points, list_enerPrimGen,list_inelasticXS);
  gr_inelasticXS -> SetLineColor(2);
  gr_inelasticXS -> Draw("LP");

  //add a legend to the graph
  leg = new TLegend(0.7,0.9,0.9,0.75);
  
  le_totXs = leg-> AddEntry(gr_totXs,"Simulated Total XS","lep");
  le_elXs = leg-> AddEntry(gr_elXs,"Simulated Elastic XS","lep");
  le_inelXs = leg-> AddEntry(gr_inelXs, "Simulated Inelastic XS","lep");
  le_totalXS = leg-> AddEntry(gr_totalXS,"Geant4 Total XS","lep");
  le_elasticXS = leg-> AddEntry(gr_elasticXS,"Geant4 Elastic XS","lep");
  le_inelasticXS = leg-> AddEntry(gr_inelasticXS, "Geant4 Inelastic XS","lep");
  leg->Draw();

  // put the graph onto a file in the folder
  c1->Print("XS_graph.png");
}

// And that's all, folks!   :)
