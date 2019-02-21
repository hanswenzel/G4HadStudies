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
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    return split(s, delim, elems);
}

using namespace std;
void readall() 
{
  const int G4sets=8;
  string  G4datasets[G4sets]=
    {"XS_anti_proton_G4_Ar.out",
     "XS_gamma_G4_Ar.out",
     "XS_kaon-_G4_Ar.out",
     "XS_kaon+_G4_Ar.out",
     "XS_neutron_G4_Ar.out",
     "XS_pi-_G4_Ar.out",
     "XS_pi+_G4_Ar.out",
     "XS_proton_G4_Ar.out"};
  //
  double ulimits[G4sets]={3500.,60.,3000.,800.,3000.,2000.,2000.,1200.};
  string  Particles[G4sets];
  TCanvas *c[G4sets];
  TGraph *grtot[G4sets];
  TGraph *grel[G4sets];
  TGraph *grinel[G4sets];
  string title[G4sets];
  string titletot[G4sets];
  string titleel[G4sets];
  string titleinel[G4sets];

  string line;
  string sstr;
  stringstream ss;
  double n;
  vector<double> en;
  vector<double> el;
  vector<double> inel;
  vector<double> tot;
  vector<double> PDGen;
  for (int i=0;i<G4sets;i++)
    {
      std::vector<std::string> y = split(G4datasets[i], '_');
      std::size_t found = G4datasets[i].find("_G4_Ar.out");
      std::size_t found1 = G4datasets[i].find("_");
      Particles[i]= G4datasets[i].substr(found1+1,found-found1-1);
    }
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
      title[i]    = Particles[i]+" on Ar Cross sections";
      titletot[i] = Particles[i]+" on Ar total hadronic Cross section";
      titleel[i]  = Particles[i]+" on Ar total elastic Cross section";
      titleinel[i]= Particles[i]+" on Ar total inelastic Cross section";
      c[i] = new TCanvas(Particles[i].c_str(),title[i].c_str(),200,10,700,500);
      c[i]->SetGrid();
      c[i]->GetFrame()->SetBorderSize(12);
      //c[i]->SetLogx();
      //c[i]->SetLogy();
      grtot[i]=new TGraph(tot.size(),enarr,totarr);
      grtot[i]->SetTitle(titletot[i].c_str());
      grtot[i]->SetMarkerColor(2);
      //      gr[i]->SetMarkerSize(2);
      grtot[i]->SetLineColor(2);
      grtot[i]->SetMarkerStyle(21);
      grtot[i]->GetXaxis()->SetLimits(10.,4000.);
      //      grtot[i]->SetRange(0.,ulimits[i] ,"Y");
      grtot[i]->SetMinimum(0.);
      grtot[i]->SetMaximum(ulimits[i]);
      grtot[i]->GetXaxis()->SetTitle("E_{kin} [MeV]");
      grtot[i]->GetYaxis()->SetTitle("#sigma [mb]");

      //grtot[i]->GetYaxis()->SetRange(0.,ulimits[i]);
      grtot[i]->Draw("AC*");
      if (Particles[i].compare("gamma")!=0)
	{
	  grel[i]=new TGraph(tot.size(),enarr,elarr);
	  grel[i]->SetTitle(titleel[i].c_str());
	  grel[i]->SetMarkerColor(3);
	  grel[i]->SetLineColor(3);
	  grel[i]->SetMarkerStyle(22);
	  grel[i]->Draw("SAME");
	  grinel[i]=new TGraph(tot.size(),enarr,inelarr);
	  grinel[i]->SetTitle(titleinel[i].c_str());
	  grinel[i]->SetMarkerColor(4);
	  grinel[i]->SetLineColor(4);
	  grinel[i]->SetMarkerStyle(23);
	  grinel[i]->Draw("SAME");
	}
      //
      TLegend *leg = c[i]->BuildLegend(.5, .7, 0.85, .85);
      leg->Draw();
      tot.clear();
      en.clear();
      el.clear();
      inel.clear();
    }
}
