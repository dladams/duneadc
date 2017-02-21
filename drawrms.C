void drawrms() {
  string sel = "Iteration$!=0&&Iteration$<4095&&Iteration$>64&&calCounts>20";
  cout << "Sel: " << sel << endl;
  TH1* ph = new TH1F("hr", "ADC bin resolution distribution (64 < ADC < 4095);Bin sigma [mV];# ADC bins [/(0.01 mV]", 100, 0.00, 1.00);
  adccalib->Draw("calRmss>>hr", sel.c_str());
  ph->SetStats(0);
  ph->SetLineWidth(2);
  ph->SetTitleOffset(1.7, "y");
  double xlab = 0.62;
  double ylab = 0.80;
  vector<string> slabs;
  {
    ostringstream sslab;
    sslab.precision(3);
    sslab << "# entries: " << ph->GetEntries()/1.e6 << " #times 10^{6}";
    slabs.push_back(sslab.str());
  }
  {
    ostringstream sslab;
    sslab.precision(2);
    sslab << "Mean: " << ph->GetMean() << " mV";
    slabs.push_back(sslab.str());
  }
  {
    int nbin = ph->GetNbinsX();
    double nover = ph->Integral(nbin+1, nbin+1);
    double ntot = ph->GetEntries();
    double fover = nover/ntot;
    double fpover = 100*fover;
    ostringstream sslab;
    sslab.precision(2);
    if ( fpover < 0.9 ) sslab.precision(1);
    sslab << "Above 1.0 mV: " << fpover << "%";
    slabs.push_back(sslab.str());
  }
  {
    int nbin = ph->GetNbinsX();
    double nover = ph->Integral(81, nbin+1);
    double ntot = ph->GetEntries();
    double fover = nover/ntot;
    double fpover = 100*fover;
    ostringstream sslab;
    sslab.precision(2);
    sslab << "Above 0.8 mV: " << fpover << "%";
    slabs.push_back(sslab.str());
  }
  TCanvas* pcan = new TCanvas;
  pcan->cd();
  pcan->SetRightMargin(0.05);
  pcan->SetLeftMargin(0.13);
  bool showovr = true;
  if ( showovr ) {
    double nmax = ph->GetBinContent(100);
    double novr = ph->GetBinContent(101);
    ph->SetBinContent(100, nmax + novr);
    ph->SetBinContent(101, 0);
  }
  ph->Draw();
  for ( string slab : slabs ) {
    TLatex* ptlab = new TLatex(xlab, ylab, slab.c_str());
    ptlab->SetNDC();
    ptlab->SetTextSize(0.04);
    ptlab->SetTextFont(42);
    ptlab->Draw();
    ylab -= 0.06;
  }
  pcan->Print("rmslow.png");
  double selfrac = ph->GetEntries()/25/16/(4096-66);
  cout << "Too few counts: " << 1 - selfrac << endl;
}
