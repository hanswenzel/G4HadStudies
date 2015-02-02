// this guy is for the data comparisons
void kaon_comp()
{

  // data
  double bugg_x[11] = { 374.4, 466.9, 537.2, 593.8, 697.4, 794.7, 890.4, 998.4, 1146.4, 1626.6, 2107.7 };
  double bugg_y[11] = { 171.6, 177.9, 187.1, 188.4, 195.4, 201.8, 202.2, 202.6,  202.7,  195.6,  188.0 };
  double bugg_ex[11] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
  double bugg_ey[11] = { 4.7, 4.3, 4.2, 4.0, 3.8, 3.7, 3.8, 3.6, 3.4, 3.2, 3.3 };

  double friedman_x[4] = { 200.5, 231.4, 327.3, 374.4 };
  double friedman_y[4] = { 162.4, 166.6, 174.9, 175.6 };
  double friedman_ex[4] = { 0.0, 0.0, 0.0, 0.0 };
  double friedman_ey[4] = { 1.9, 1.3, 0.8, 0.9 };

  double friedman_inel_y[4] = { 120.4, 129.3, 141.8, 149.3 };
  double friedman_inel_ey[4] = { 2.3, 1.4, 1.5, 1.5 };

  TGraphErrors * bugg = new TGraphErrors( 11, bugg_x, bugg_y, bugg_ex, bugg_ey );
  TGraphErrors * friedman = new TGraphErrors( 4, friedman_x, friedman_y, friedman_ex, friedman_ey );
  TGraphErrors * friedman_inel = new TGraphErrors( 4, friedman_x, friedman_inel_y, friedman_ex, friedman_inel_ey );

  // GEANT predictions
  TFile * ftfp = new TFile( "ftfp.root", "OLD" );
  TGraph * geant_ftfp = (TGraph*) ftfp->Get("total");
  TFile * qgsp = new TFile( "qgsp.root", "OLD" );
  TGraph * geant_qgsp = (TGraph*) qgsp->Get("total");

  geant_ftfp->SetLineColor( kRed );
  geant_qgsp->SetLineColor( kBlue );
  geant_ftfp->SetMarkerColor( kRed );
  geant_qgsp->SetMarkerColor( kBlue );
  geant_ftfp->SetMarkerSize( 0.4 );
  geant_qgsp->SetMarkerSize( 0.4 );
//  bugg->SetMarkerSize( 0.6 );
  bugg->SetMarkerStyle( 21 );
  friedman->SetMarkerStyle( 22 );
//  friedman->SetMarkerSize( 0.6 );

  // draw them all together
  TCanvas * c = new TCanvas();

  geant_ftfp->SetMinimum(100.0);
  geant_ftfp->SetMaximum(350.0);
  geant_ftfp->Draw("APC");
  geant_qgsp->Draw("PC same");

  bugg->Draw("P same");
  friedman->Draw("P same");

  TLegend * leg = new TLegend( 0.5, 0.6, 0.845, 0.845 );
  leg->AddEntry( bugg, "Bugg data", "pe" );
  leg->AddEntry( friedman, "Friedman data", "pe" );
  leg->AddEntry( geant_ftfp, "FTFP_BERT", "l" );
  leg->AddEntry( geant_qgsp, "QGSP_BERT", "l" );
  leg->Draw();
  c->RedrawAxis();

  c->Print("xs_comparison.png");
  delete c;

}
