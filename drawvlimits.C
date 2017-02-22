void drawvlimits(bool same, vector<unsigned int> chips) {
  const string myname = "drawvlimits: ";
  TTree* adccalib = dynamic_cast<TTree*>(gDirectory->Get("adccalib"));
  if ( adccalib == nullptr ) {
    cout << myname << "Unable to find tree adccalib." << endl;
    return;
  }
  string sadcmin = "64";
  string sadcmax = "4095";
  int msty = adccalib->GetMarkerStyle();
  string stitl = "Input voltage limits for " + sadcmin + " < ADC < " + sadcmax + "; Channel; V_{in} [mV]";
  TH1* ph = new TH2F("hvlimits", stitl.c_str(), 25, 0, 400, 20, -200, 1800);
  ph->SetStats(0);
  ph->GetYaxis()->SetTickLength(0.01);
  TCanvas* pcan = nullptr;
  if ( ! same ) {
    pcan = new TCanvas("cvli", "cvli", 1500, 500);
    pcan->SetGridy();
    pcan->SetRightMargin(0.02);
    pcan->SetLeftMargin(0.05);
    ph->SetTitleOffset(0.7, "y");
    drawChipBounds(ph, true, true, true, chips);
    //ph->DrawCopy();
  }
  // Create drawing arguments and selections.
  vector<string> sarg1s;
  vector<string> sarg2s;
  vector<string> ssels;
  if ( chips.size() == 0 ) {
    sarg1s.push_back("gain*" + sadcmin + "+offset:Entry$+0.5");
    sarg2s.push_back("gain*" + sadcmax + "+offset:Entry$+0.5");
    ssels.push_back("");
  }
  for ( unsigned int ichp=0; ichp<chips.size(); ++ichp ) {
    unsigned int chip = chips[ichp];
    ostringstream sschanoff;
    sschanoff << 16*ichp;
    ostringstream sschip;
    sschip << chip;
    string schip = sschip.str();
    string schanoff = sschanoff.str();
    sarg1s.push_back("gain*" + sadcmin + "+offset:" + schanoff + "+chan+0.5");
    sarg2s.push_back("gain*" + sadcmax + "+offset:" + schanoff + "+chan+0.5");
    ssels.push_back("chip==" + schip);
  }
  // Loop over draws.
  for ( unsigned int idrw=0; idrw<ssels.size(); ++idrw ) {
    adccalib->SetMarkerStyle(7);
    string sarg = "gain*" + sadcmin + "+offset:Entry$+0.5";
    adccalib->Draw(sarg1s[idrw].c_str(), ssels[idrw].c_str(), "same");
    adccalib->SetMarkerStyle(6);
    adccalib->Draw(sarg2s[idrw].c_str(), ssels[idrw].c_str(), "same");
    adccalib->SetMarkerStyle(msty);
  }
  if ( pcan != nullptr ) pcan->Print("vlimits.png");
}
