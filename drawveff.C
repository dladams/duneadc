void drawveff() {
  string myname = "drawveff: ";
  TTree* ptree = dynamic_cast<TTree*>(gDirectory->Get("adcperf"));
  if ( ptree == nullptr ) {
    cout << myname << "Performance tree not found." << endl;
    return;
  }
  AdcVoltagePerformance* pavp = nullptr;
  ptree->SetBranchAddress("perf", &pavp);
  ptree->GetEntry(0);
  unsigned int nv = pavp->nv;
  float vmin = pavp->vmin;
  float vmax = pavp->vmax;
  float vrmsmax = pavp->vrmsmax;
  if ( nv == 0 ) {
    cout << myname << "First performance object has no voltages." << endl;
    return;
  }
  cout << myname << "# voltages: " << nv << endl;
  cout << myname << "Voltage range: (" << vmin << ", " << vmax << ") mV" << endl;
  cout << myname << "Voltage limit: " << vrmsmax << " mV" << endl;
  unsigned int ncha = ptree->GetEntries();
  cout << myname << "# channels: " << ncha << endl;
  // Create histogram.
  ostringstream sstitl;
  sstitl << "Input voltage efficiency for V_{in} RMS < " << vrmsmax << " mV; Channel; V_{in} [mV]";
  string stitl = sstitl.str();
  TH2* ph = new TH2F("hveffsum", stitl.c_str(), ncha, 0, ncha, nv, vmin, vmax);
  ph->SetStats(0);
  ph->SetContour(20);
  ph->SetStats(0);
  ph->GetYaxis()->SetTickLength(0.01);
  // Fill histogram.
  unsigned int icha1 = 0;
  unsigned int icha2 = ncha;
  for ( unsigned int icha=icha1; icha<icha2; ++icha ) {
    ptree->GetEntry(icha);
    if ( pavp->nv != nv ) {
      cout << myname << "Entry " << icha << " is inconsistent." << endl;
      return;
    }
    for ( unsigned int iv=0; iv<nv; ++iv ) {
      unsigned int bin = ph->GetBin(icha+1, iv+1);
      double eff = pavp->eff(iv);
      if ( eff > 0.50 ) {
        ph->SetBinContent(bin, pavp->eff(iv));
      }
    }
  }
  // Draw histogram.
  palette(41);
  TCanvas* pcan = new TCanvas("veff", "veff", 1500, 500);
  pcan->SetGridy();
  pcan->SetRightMargin(0.06);
  pcan->SetLeftMargin(0.05);
  ph->SetTitleOffset(0.7, "y");
  drawChipBounds(ph, true, true, true);
  //ph->DrawCopy("colz");
  if ( 1 ) {
    TFile::Open("calib_201701.root");
    gROOT->ProcessLine(".X drawvlimits.C(true)");
  }
  pcan->Print("veff.png");
}
