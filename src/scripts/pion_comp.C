void mark( TGraph * graph, int color )
{
  graph->SetLineColor( color );
  graph->SetMarkerColor( color );
  graph->SetMarkerSize( 0.4 );
}

void minmax( TGraph * graph, double &min, double &max )
{

  for( int p = 0; p < graph->GetN(); ++p ) {
    double x, y;
    graph->GetPoint( p, x, y );
    if( y < min ) min = y;
    if( y > max ) max = y;
  }
}

// this guy is for the model comparisons
void pion_comp()
{

  // GEANT predictions
  TFile * ftfp_file = new TFile( "qgsp_nobert_pions.root", "OLD" );
  TFile * qgsp_file = new TFile( "qgsp_pions.root", "OLD" );

  std::string names[5] = { "total", "elastic", "inelastic", "cx", "kplus" };
  std::string tits[5] = { "Total", "Elastic", "Other inelastic", "CX", "K^{+} production" };

  TGraph * ftfp[5];
  TGraph * qgsp[5];

  for( int i = 0; i < 5; ++i ) {
    ftfp[i] = (TGraph*) ftfp_file->Get(names[i].c_str());
    qgsp[i] = (TGraph*) qgsp_file->Get(names[i].c_str());
    mark( ftfp[i], 2 );
    mark( qgsp[i], 4 );
  }

  TLegend * leg = new TLegend( 0.55, 0.65, 0.846, 0.846 );
  leg->AddEntry( ftfp[0], "QGSP", "l" );
  leg->AddEntry( qgsp[0], "QGSP_BERT", "l" );

  // draw them all together
  TCanvas * c = new TCanvas();
  for( int i = 0; i < 5; ++i ) {
    double min = 1.0E30;
    double max = 0.0;
    minmax( ftfp[i], min, max );
    minmax( qgsp[i], min, max );

    qgsp[i]->SetTitle( Form(";#pi^{+} KE (MeV);%s", tits[i].c_str()) );
    qgsp[i]->SetMinimum( min - (max-min)*0.1 );
    qgsp[i]->SetMaximum( max + (max-min)*0.7 );

    qgsp[i]->Draw("APC");
    ftfp[i]->Draw("PC same");
    leg->Draw();
    c->RedrawAxis();
    c->Print( Form("pion_%s_comp.png", names[i].c_str()) );
  }

  delete c;

}
