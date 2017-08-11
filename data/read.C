// reading a text file
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector> 
#include <cmath>
#include <TCanvas.h> 
#include <TMultiGraph.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TFrame.h>
using namespace std;
void read() 
{
  const int G4sets=7;
  string  G4datasets[G4sets]=
    {"G4_gammap.txt",
     "G4_kaon-p.txt",
     "G4_kaon+p.txt",
     "G4_pbarp.txt",
     "G4_pi-p.txt",
     "G4_pi+p.txt",
     "G4_pp.txt"};
  //
  string  Particles[G4sets]= {"gamma","kaon-","kaon+","pbar","pi-","pi+","p"};
  string  PDGdatasetstotal[G4sets]={
    "rpp2014-gammap_total.dat",
    "rpp2014-kmp_total.dat",
    "rpp2014-kpp_total.dat",
    "rpp2014-pbarp_total.dat",
    "rpp2014-pimp_total.dat",
    "rpp2014-pipp_total.dat",
    "rpp2014-pp_total.dat"};
  TCanvas *c[G4sets];
  TGraph *gr[G4sets];
  string title[G4sets];
  TGraphErrors *PDGgr[G4sets];
  string PDGtitle[G4sets];
  //
  string PDGdatasetstelastic[6]={
    "rpp2014-kmp_elastic.dat",
    "rpp2014-kpp_elastic.dat",
    "rpp2014-pbarp_elastic.dat",
    "rpp2014-pimp_elastic.dat",
    "rpp2014-pipp_elastic.dat",
    "rpp2014-pp_elastic.dat"};
  //
  double pmasses[G4sets]={0.0,0.49368,0.49368,0.9382720813,0.13957061,0.13957061,0.9382720813};
  string line;
  string sstr;
  stringstream ss;
  double n;
  vector<double> en;
  vector<double> el;
  vector<double> inel;
  vector<double> tot;
  vector<double> PDGen;
  //  vector<double> PDGel;
  // vector<double> PDGinel;
  vector<double> PDGtot;
  vector<double> PDGtoterr;
  for (int i=0;i<G4sets;i++)
    {
      ifstream myfile (G4datasets[i]);
      if (myfile.is_open())
	{
	  while ( getline (myfile,line) )
	    {
	      sstr =  line.substr(0,1);
	      if (sstr !="-"&&sstr !="*")
		{
		  int nw= 0;
		  ss.clear(); 
		  ss.str(line);
		  while (ss >> n) {
		    if (nw==1){
		      en.push_back(n);
		    } else if  (nw==2){
		      el.push_back(1000*n);
		    }
		    else if  (nw==3){
		      inel.push_back(1000*n);
		    }else if  (nw==4){
		      tot.push_back(1000*n);
		    }
		    nw++;
		  }
		}
	    }
	}
      else 
	{
	  cout << "Unable to open file"; 
	}
      myfile.close();
      double enarr[tot.size()];
      std::copy(en.begin(), en.end(), enarr);
      double elarr[tot.size()];
      std::copy(el.begin(), el.end(), elarr);
      double inelarr[tot.size()];
      std::copy(inel.begin(), inel.end(), inelarr);
      double totarr[tot.size()];
      std::copy(tot.begin(), tot.end(), totarr);
      title[i] = Particles[i]+"  on  p total cross section 4.10.3.p01 FTFP_BERT";
      c[i] = new TCanvas(Particles[i].c_str(),title[i].c_str(),200,10,700,500);
      c[i]->SetGrid();
      c[i]->GetFrame()->SetBorderSize(12);
      c[i]->SetLogx();
      c[i]->SetLogy();
      gr[i]=new TGraph(tot.size(),enarr,totarr);
      gr[i]->SetTitle(title[i].c_str());
      gr[i]->SetMarkerColor(2);
      //      gr[i]->SetMarkerSize(2);
      gr[i]->SetLineColor(2);
      gr[i]->SetMarkerStyle(21);
      gr[i]->GetXaxis()->SetLimits(10.,700000.);
      gr[i]->Draw("AC*");
      en.clear();
      el.clear();
      inel.clear();
      tot.clear();
      
      // now deal with PDG.
      ifstream myfilepdg (PDGdatasetstotal[i]);
      //cout<<PDGdatasetstotal[i]<<endl;

      if (myfilepdg.is_open())
	{
	  while ( getline (myfilepdg,line) )
	    {
	      sstr =  line.substr(0,1);
	      if (sstr !="#")
		{
		  int nw= 0;
		  ss.clear(); 
		  ss.str(line);
		  while (ss >> n) {
		    if (nw==1){
		      double plab = n;
		      double ekin=1000*(sqrt(plab*plab +  pmasses[i]*pmasses[i])-pmasses[i]);
		      PDGen.push_back(ekin);
		    } else if  (nw==4){
		      //cout <<n<<endl;
		      PDGtot.push_back(n);
		    }else if  (nw==5){
		      PDGtoterr.push_back(n);
		    }
		    nw++;
		  }
		}
	    }
	}
      else cout << "Unable to open file"; 
      myfilepdg.close();
      
      double PDGenarr[PDGtot.size()];
      std::copy(PDGen.begin(), PDGen.end(), PDGenarr);
      double PDGenerrarr[PDGtot.size()];
      for (unsigned int ii=0;ii<PDGtot.size();ii++)
	{
	  PDGenerrarr[ii]=0.0;
	}
      double PDGtotarr[PDGtot.size()];
      std::copy(PDGtot.begin(), PDGtot.end(), PDGtotarr);
      double PDGtoterrarr[PDGtoterr.size()];
      std::copy(PDGtoterr.begin(), PDGtoterr.end(), PDGtoterrarr);
      PDGtitle[i] = Particles[i]+"  on  p total cross section (experiment)";

      PDGgr[i]=new TGraphErrors(PDGtot.size(),PDGenarr,PDGtotarr,PDGenerrarr,PDGtoterrarr);
      PDGgr[i]->SetTitle(PDGtitle[i].c_str());
      PDGgr[i]->SetMarkerColor(4);
      PDGgr[i]->SetLineColor(4);
      PDGgr[i]->SetMarkerStyle(20);
      PDGgr[i]->SetMarkerSize(2);
      PDGgr[i]->SetLineWidth (2);
      PDGgr[i]->Draw("SAME");
      //
      PDGen.clear();
      PDGtot.clear();
      PDGtoterr.clear();
      //
      TLegend *leg = c[i]->BuildLegend(.5, .1, 0.9, .3);
      leg->Draw();
    }
}
