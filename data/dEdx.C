#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector> 
#include <cmath>
#include <TCanvas.h> 
#include <TGraph.h>
#include <TLegend.h>
#include <TAxis.h>
#include <TFrame.h>
#include <TFile.h>
using namespace std;
//-------------------------------------------------------------------------------
//
// see formula 33.5 in Review of particle physics 2018
// Bethe equation 
//
// m:    mass of particle in MeV
// q:    charge of particle
// plab: momentum
// ro:   density of material in g/cm3
// Z:    Z of material
// A:    Atomic number of material
// I:    mean excitation energy
// 
// the PDG data is taken from:
//  http://pdg.lbl.gov/2018/AtomicNuclearProperties
//-------------------------------------------------------------------------------
double stoppingPower(double m, double q, double plab, double ro, double Z, double A, double I) {

	const double k = 0.307075;
	double g, b2, T, SP;
	double E =sqrt(plab*plab +  m*m)-m;

	//printf("\n|******** Stopping-Power values for this material ********|\n");

	g = 1 + (E/m);
	//printf("\n Gamma  =  %lf", g);

	b2 = 1 - (1/(g*g));
	//printf("\n Beta squared  =  %lf", b2);

	T = (1.022*b2*g*g) / (1 + 2*g*(0.511/m) + pow(0.511/m,2) );
	//printf("\n Maximum kinetic energy transferred  =  %lf MeV ", T);

	SP = ((ro*k*q*q*Z)/(A*b2))*(0.5*log( (1.022*b2*g*g*T) / (I*I) ) - b2);
	//printf("\n Stopping Power  =  %lf MeV/cm \n", SP);
	//printf("\n Stopping Power  =  %lf MeV cm^2/g \n", SP/ro);
	return SP/ro;
}



void dEdx() 
{
  string line;
  string mmi ="Minimum ionization";
  string mce="Muon critical energy";
  string header= "-----------------------";
  ifstream myfile ("muE_liquid_argon.txt");
  bool head = true;
  vector<double> momentum;
  vector<double> ion;
  vector<double> bbion;                 // using Bethe Bloch formula to calculate Ionization loss
  vector<double> brems;
  vector<double> pair;
  vector<double> photonuclear;
  vector<double> radloss;               // sum: brems+pair+photonuclear
  vector<double> dEdx;
  vector<double> csda;
  vector<double> length;
  // muon properties:
  const double mumass=105.6583745;      // muon mass in MeV
  const double mucharge=-1.0;            // mu- charge
              
  // liquid Argon properties
  const double density = 1.396;         // density of liquid Ar in g/cm^3 
  const double Z=18.;                   // Atomic number
  const double A=39.948;                // Atomic mass
  const double I=0.000188;              // Mean excitation energy in MeV

  if (myfile.is_open())
    {
      while ( getline (myfile,line) )
	{

	  if (!head){
	    std::size_t foundmmi=line.find(mmi);
	    std::size_t foundmce=line.find(mce);
	    if (foundmmi==std::string::npos&&foundmce==std::string::npos) 
	      {
		std::istringstream iss(line);
		std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
						 std::istream_iterator<std::string>());
		std::stringstream ss;
		ss << results.at(1);
		double val;
		ss >> val;
		momentum.push_back(val);
		ss.str("");
		ss.clear();
		ss << results.at(2);
		ss >> val;
		ion.push_back(val);
		ss.str("");
		ss.clear();
		ss << results.at(3);
		ss >> val;
		brems.push_back(val);
		ss.str("");
		ss.clear();
		ss << results.at(4);
		ss >> val;
		pair.push_back(val);
		ss.str("");
		ss.clear();
		ss << results.at(5);
		ss >> val;
		photonuclear.push_back(val);
		ss.str("");
		ss.clear();
		ss << results.at(6);
		ss >> val;
		radloss.push_back(val);
		ss.str("");
		ss.clear();
		ss << results.at(7);
		ss >> val;
		dEdx.push_back(val);
		ss.str("");
		ss.clear();
		ss << results.at(8);
		ss >> val;
		csda.push_back(val);
		length.push_back(val/density);
		  //		std::cout <<results.at(1)<<"  "<< results.at(11)<<std::endl;
	      }
	  }
	  else {
	    //	    std::cout<<line<<std::endl;
	    std::size_t found=line.find(header);
	    if (found!=std::string::npos)
	      {
		head = false;
		//std::cout << "'-----------------------' also found at: " << found << '\n';
	      }
	  }
	}
    }
  else 
    {
      cout << "Unable to open file"; 
      exit(EXIT_FAILURE);
    }
  for(std::vector<double>::iterator it = momentum.begin(); it != momentum.end(); ++it) {
    //std::cout << *it<<"    "<<stoppingPower(mumass, mucharge,*it, density, Z, A, I)<<endl;
    bbion.push_back(stoppingPower(mumass, mucharge,*it, density, Z, A, I));
  }
  double bbionarr[ion.size()];
  std::copy(bbion.begin(), bbion.end(), bbionarr);
  double momentumarr[momentum.size()];
  std::copy(momentum.begin(), momentum.end(), momentumarr);
  double ionarr[ion.size()];
  std::copy(ion.begin(), ion.end(), ionarr);
  double bremsarr[brems.size()];
  std::copy(brems.begin(), brems.end(), bremsarr);
  double pairarr[pair.size()];
  std::copy(pair.begin(), pair.end(), pairarr);
  double photonucleararr[photonuclear.size()];
  std::copy(photonuclear.begin(), photonuclear.end(), photonucleararr);
  double radlossarr[radloss.size()];
  std::copy(radloss.begin(), radloss.end(), radlossarr);
  double dEdxarr[dEdx.size()];
  std::copy(dEdx.begin(), dEdx.end(), dEdxarr);
  double csdaarr[csda.size()];
  std::copy(csda.begin(), csda.end(), csdaarr);
  double lengtharr[length.size()];
  std::copy(length.begin(), length.end(), lengtharr);

  TCanvas* c = new TCanvas("dEdx canvas","dE/dx",200,10,700,500);
  c->SetGrid();
  c->GetFrame()->SetBorderSize(12);
  c->SetLogx();
  c->SetLogy();
  TGraph* dEdxgr=new TGraph(momentum.size(),momentumarr,dEdxarr);
  dEdxgr->SetTitle("PDG liquid Ar dEdx");
  dEdxgr->SetName("dEdxgr");
  dEdxgr->SetMarkerColor(4);
  dEdxgr->SetLineColor(4);
  dEdxgr->SetMarkerStyle(20);
  dEdxgr->SetMarkerSize(1);
  dEdxgr->SetLineWidth (1);
  dEdxgr->GetXaxis()->SetTitle("P [MeV/c]");
  dEdxgr->GetYaxis()->SetTitle("dEdx [MeV cm^{2}/g]");
  dEdxgr->Draw();
  TGraph* iongr=new TGraph(momentum.size(),momentumarr,ionarr);
  iongr->SetTitle("PDG liquid Ar ion");
  iongr->SetName("bbiongr");
  iongr->SetMarkerColor(3);
  iongr->SetLineColor(3);
  iongr->SetMarkerStyle(21);
  iongr->SetMarkerSize(1);
  iongr->SetLineWidth (1);
  iongr->Draw("SAME");
  TGraph* bbiongr=new TGraph(momentum.size(),momentumarr,bbionarr);
  bbiongr->SetTitle("liquid Ar ion (Bethe Bloch)");
  bbiongr->SetName("iongr");
  bbiongr->SetMarkerColor(2);
  bbiongr->SetLineColor(2);
  bbiongr->SetMarkerStyle(21);
  bbiongr->SetMarkerSize(1);
  bbiongr->SetLineWidth (1);
  bbiongr->Draw("SAME");
  TGraph* bremsgr=new TGraph(momentum.size(),momentumarr,bremsarr);
  bremsgr->SetTitle("PDG liquid Ar brems");
  bremsgr->SetName("bremsgr");
  bremsgr->SetMarkerColor(13);
  bremsgr->SetLineColor(13);
  bremsgr->SetMarkerStyle(22);
  bremsgr->SetMarkerSize(1);
  bremsgr->SetLineWidth (1);
  bremsgr->Draw("SAME");




  TGraph* pairgr=new TGraph(momentum.size(),momentumarr,pairarr);
  pairgr->SetTitle("PDG liquid Ar pair");
  pairgr->SetName("pairgr");
  pairgr->SetMarkerColor(18);
  pairgr->SetLineColor(18);
  pairgr->SetMarkerStyle(23);
  pairgr->SetMarkerSize(1);
  pairgr->SetLineWidth (1);
  pairgr->Draw("SAME");
  TGraph* photonucleargr=new TGraph(momentum.size(),momentumarr,photonucleararr);
  photonucleargr->SetTitle("PDG liquid Ar photonuclear");
  photonucleargr->SetName("photonucleargr");
  photonucleargr->SetMarkerColor(5);
  photonucleargr->SetLineColor(5);
  photonucleargr->SetMarkerStyle(24);
  photonucleargr->SetMarkerSize(1);
  photonucleargr->SetLineWidth (1);
  photonucleargr->Draw("SAME");
  TGraph* radlossgr=new TGraph(momentum.size(),momentumarr,radlossarr);
  radlossgr->SetTitle("PDG liquid Ar radloss");
  radlossgr->SetName("radlossgr");
  radlossgr->SetMarkerColor(7);
  radlossgr->SetLineColor(7);
  radlossgr->SetMarkerStyle(25);
  radlossgr->SetMarkerSize(1);
  radlossgr->SetLineWidth (1);
  radlossgr->Draw("SAME");
  TLegend *leg = c->BuildLegend(.2, .65, 0.5, .85);
  leg->Draw();

  TCanvas* ci = new TCanvas("Ionization canvas","dE/dx",200,10,700,500);
  ci->SetGrid();
  ci->GetFrame()->SetBorderSize(12);
  ci->SetLogx();
  ci->SetLogy();
  iongr->Draw();
  bbiongr->Draw("SAME");
  TLegend *ileg = ci->BuildLegend(.2, .65, 0.5, .85);
  ileg->Draw();
  TCanvas* c2 = new TCanvas("csda canvas","csda",200,10,700,500);
  c2->SetGrid();
  c2->GetFrame()->SetBorderSize(12);
  c2->SetLogx();
  c2->SetLogy();
  TGraph* csdagr=new TGraph(momentum.size(),momentumarr,csdaarr);
  csdagr->SetTitle("PDG liquid Ar csda range");
  csdagr->SetName("csdagr");
  csdagr->SetMarkerColor(4);
  csdagr->SetLineColor(4);
  csdagr->SetMarkerStyle(25);
  csdagr->SetMarkerSize(1);
  csdagr->SetLineWidth (1);
  csdagr->GetXaxis()->SetTitle("P [MeV/c]");
  csdagr->GetYaxis()->SetTitle("CSDA [g/cm^{2}]");
  csdagr->Draw();
  TCanvas* c3 = new TCanvas("length canvas","length",200,10,700,500);
  c3->SetGrid();
  c3->GetFrame()->SetBorderSize(12);
  c3->SetLogx();
  c3->SetLogy();
  TGraph* lengthgr=new TGraph(momentum.size(),momentumarr,lengtharr);
  lengthgr->SetTitle("range");
  lengthgr->SetName("lengthgr");
  lengthgr->SetMarkerColor(4);
  lengthgr->SetLineColor(4);
  lengthgr->SetMarkerStyle(25);
  lengthgr->SetMarkerSize(1);
  lengthgr->SetLineWidth (1);
  lengthgr->GetXaxis()->SetTitle("P [MeV/c]");
  lengthgr->GetYaxis()->SetTitle("range [cm]");
  lengthgr->Draw();

  TFile *_file0 = TFile::Open("dEdx.root", "RECREATE" );
  _file0->cd();
  dEdxgr->Write();
  iongr->Write();
  bremsgr->Write();
  pairgr->Write();
  photonucleargr->Write();
  radlossgr->Write();
  csdagr->Write();
  lengthgr->Write();
  _file0->Close();
}
