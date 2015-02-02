//====================================================================                   
//C script for total cross section calculation.                                           
// Juan Pablo Velásquez Ormaeche                                                         
//HEP-PUCP                                                                               
//Last modification date: August 22, 2014.  
// Then Chris Marshall, 23 October 2014                                           
//====================================================================                   

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

std::string tuple_list = "tuplelist.txt";
std::string material_name = "C";

FILE * fout = fopen("out.txt", "w");

TGraph * total;
TGraph * elastic;
TGraph * inelastic;
TGraph * cx;
std::string names[7] = {"K+", "K0", "p", "n", "gamma", "meson", "fragment"};
TGraph * particles[7];

void settings( std::string material_name, double &atomic_mass )
{
  //Selecting target material atomic mass                                                          
  if      (material_name == "C") atomic_mass = 12.0; //  C                                                
  else if (material_name == "Fe") atomic_mass = 55.8452;  //Fe                                             
  else if (material_name == "Pb") atomic_mass = 207.21; //Pb                                               
  else if (material_name == "Bi") atomic_mass = 208.98040; //Bi                                            
  else if (material_name == "Ni") atomic_mass = 58.6934; //Ni                                              
  else if (material_name == "Cu") atomic_mass = 63.546; //Cu                                               
  else if (material_name == "Ta") atomic_mass = 180.9479; //Ta                                             
  else if (material_name == "Si") atomic_mass =  28.0855; //Si                                             
  else if (material_name == "lH2") atomic_mass = 2.02; //lH2
}


void total_XS( std::string filename, std::string material_name )
{

  int n_elastic = 0;
  int n_inelastic = 0;
  int n_cx = 0;

  double atomic_mass;
  settings( material_name, atomic_mass );

  TFile *f = new TFile( filename.c_str(), "OLD" );
  TTree *T = (TTree *) f->Get("nTarget");

  int npart, numberEvts;
  double elasticXS, inelasticXS, thickness, density, radius, enerPrimGen;
  int pdg[100], intType[100];
  double x[100][3], p[100][4]; // filled once per particle (nPart)
  //const double pi = 3.14159265;
  const double Na = 6.023*pow(10,23);

  T->SetBranchAddress("npart", &npart);
  T->SetBranchAddress("pdg", pdg);
  T->SetBranchAddress("x", x);
  T->SetBranchAddress("p", p);
  T->SetBranchAddress("intType",intType);

  TTree *T2 = (TTree *) f->Get("RunInfo");
  T2->SetBranchAddress("density", &density);
  T2->SetBranchAddress("elasticXS", &elasticXS);
  T2->SetBranchAddress("inelasticXS", &inelasticXS);
  T2->SetBranchAddress("enerPrimGen", &enerPrimGen);
  T2->SetBranchAddress("numberEvts", &numberEvts);
  T2->SetBranchAddress("tickness", &thickness); // target thickness, in mm
  T2->SetBranchAddress("radius", &radius);
  T2->GetEntry(0); // the only entry

  int n_entries = T->GetEntries();

  double counts[7] = {0.0};

  double k_ke = 0.0;
  int n_kplus = 0;

  for( int i = 0; i < n_entries; ++i ) {
    T->GetEntry(i);

    if( npart == 1 && intType[0] == 2 ) n_elastic++;
    else {
      n_inelastic++;

      for( int j = 0; j < npart; ++j ) {
        
        if( pdg[j] == 130 || pdg[j] == 310 ) n_cx++;
        if( pdg[j] == 321 ) {
          k_ke += sqrt( p[j][0]*p[j][0] + p[j][1]*p[j][1] + p[j][2]*p[j][2] + 493.7*493.7 ) - 493.7;
          counts[0]++;
        } else if( pdg[j] == 130 || pdg[j] == 310 ) counts[1]++;
        else if( pdg[j] == 2212 ) counts[2]++;
        else if( pdg[j] == 2112 ) counts[3]++;
        else if( pdg[j] == 22 ) counts[4]++;
        else if( pdg[j] < 9999 ) counts[5]++;
        else counts[6]++;

        
      }

    }

  }

  for( int i = 0; i < 7; ++i ) counts[i] /= n_inelastic;

  k_ke /= (n_inelastic-n_cx);
  k_ke /= enerPrimGen;

  // convert number of events to a cross section
  double surviveNum_total  = numberEvts - n_inelastic - n_elastic;
  double total_int = n_elastic + n_inelastic;

  double convert_to_cm2_per_C_atom = atomic_mass / (density*Na*thickness*0.1);

  double total_XS = log(numberEvts/surviveNum_total) * convert_to_cm2_per_C_atom * pow(10,27); // in mb
  //double error_total_XS = pow(total_int,0.5) * convert_to_cm2_per_C_atom / surviveNum_total * pow(10,27);
  double inelastic_XS = (n_inelastic/total_int)*total_XS;
  double elastic_XS = (n_elastic/total_int)*total_XS;
  double cx_XS = (n_cx/total_int)*total_XS;
  double kplus_XS = (n_kplus/total_int)*total_XS;

  printf( "Kinetic energy = %2.0f Total XS = %2.1f inel = %2.1f elastic = %2.1f CX = %2.1f from file inel %2.1f elastic %2.1f\n", enerPrimGen, total_XS, inelastic_XS, elastic_XS, cx_XS, inelasticXS, elasticXS );

  int n = total->GetN();
  total->SetPoint( n, enerPrimGen, total_XS );
  elastic->SetPoint( n, enerPrimGen, elastic_XS );
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  inelastic->SetPoint( n, enerPrimGen, inelasticXS/*inelastic_XScx_XS*/ );
  // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  cx->SetPoint( n, enerPrimGen, cx_XS );

  for( int i = 0; i < 7; ++i ) particles[i]->SetPoint( n, enerPrimGen, counts[i] );

}

void kaon_xs()
{

  int colors[7] = { kBlack, kRed, kBlue, kGreen+2, kMagenta+3, kOrange+1, kCyan+2 };

  for( int i = 0; i < 7; ++i ) {
    particles[i] = new TGraph();
    particles[i]->SetName( names[i].c_str() );
    particles[i]->SetTitle( names[i].c_str() );
  }
  total = new TGraph();
  total->SetName("total");
  total->SetTitle(";Kaon KE (MeV);Total XS (mb)");
  elastic = new TGraph();
  elastic->SetName("elastic");
  elastic->SetTitle(";Kaon KE (MeV);Elastic XS (mb)");
  inelastic = new TGraph();
  inelastic->SetName("inelastic");
  inelastic->SetTitle(";Kaon KE (MeV);Inelastic XS (mb)");
  cx = new TGraph();
  cx->SetName("cx");
  cx->SetTitle(";Kaon KE (MeV);Charge exchange XS (mb)");


  ifstream  tp_list(tuple_list.c_str());
  std::string tuple;

  while( tp_list >> tuple ) {
    std::vector<double> XS_vector;
    total_XS( tuple, material_name );
  }

  elastic->SetLineColor(kRed);
  elastic->SetMarkerColor(kRed);
  //inelastic->SetLineColor(kBlue);
  //inelastic->SetMarkerColor(kBlue);
  cx->SetLineColor(kGreen+2);
  cx->SetMarkerColor(kGreen+2);
  total->SetMarkerSize(0.4);
  elastic->SetMarkerSize(0.4);
  inelastic->SetMarkerSize(0.4);
  cx->SetMarkerSize(0.4);

  total->SetMinimum(0.0);
  total->SetMaximum(400.0);
  inelastic->SetMinimum(70.0);
  inelastic->SetMaximum(150.0);

  TLegend leg = TLegend( 0.5, 0.6, 0.846, 0.846 );
  leg.AddEntry( total, "Total", "l" );
  leg.AddEntry( elastic, "Elastic", "l" );
  leg.AddEntry( cx, "CX", "l" );
  leg.AddEntry( inelastic, "Other inelastic", "l" );

  TCanvas * c = new TCanvas();
  //total->Draw("APC");
  //elastic->Draw("PC same");
  //inelastic->Draw("PC same");
  inelastic->Draw("APC");
  //cx->Draw("PC same");
  //leg.Draw();
  c->RedrawAxis();
  c->Print("cross_section.png");
  delete c;

  particles[0]->SetTitle(";Kaon kinetic energy (MeV);Particles per inelastic interaction");
  particles[0]->SetMinimum(0);
  particles[0]->SetMaximum(3.0);
  TCanvas * c2 = new TCanvas();
  TLegend leg2 = TLegend( 0.55, 0.6, 0.846, 0.846 );
  for( int i = 0; i < 7; ++i ) {
    particles[i]->SetLineColor( colors[i] );
    particles[i]->SetMarkerColor( colors[i] );
    particles[i]->SetMarkerSize(0.4);
    leg2.AddEntry( particles[i], names[i].c_str(), "l" );
    if( i == 0 ) particles[i]->Draw("APC");
    else particles[i]->Draw("PC same");
  }
  leg2.Draw();
  c2->RedrawAxis();
  c2->Print("particles.png");
  delete c2;

  TFile * fout = new TFile( "kaon_xs.root", "RECREATE" );
  fout->cd();
  total->Write();
  inelastic->Write();
  elastic->Write();
  cx->Write();


}


