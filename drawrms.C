void drawrms(int adcmin =65, int adcmax=4095, string sstat ="all", bool doLinear =false) {
  ostringstream sssel;
  ostringstream sstitl;
  sssel << "Iteration$>" << adcmin << "&&Iteration$<" << adcmax << "&&calCounts>20";
  sstitl << "ADC bin resolution (" << adcmin << " < ADC < " << adcmax;
  string hname = "hr";
  string fname = "rmslow_";
  if ( sstat == "good" ) {
    sssel << "&&AdcChipStatus::good(chip)";
    sstitl << ", good";
    hname += "good";
    fname += "good";
  } else {
    hname += "all";
    fname += "all";
  }
  sstitl << ")";
  int nx = 100;
  double xmax = 1.0;
  string sarg = "calRmss";
  if ( doLinear ) {
    sstitl << " for linear calib";
    hname += "lin";
    fname += "_linear";
    xmax = 10.0;
    sarg = "sqrt(calRmss*calRmss + (calMeans-gain*Iteration$-offset)*(calMeans-gain*Iteration$-offset))";
  }
  fname += ".png";
  sstitl << ";Bin sigma [mV];# ADC bins [/(" << xmax/nx << " mV]";
  string sel = sssel.str();
  string titl = sstitl.str();
  cout << "Sel: " << sel << endl;
  sarg += ">>" + hname;
  TH1* ph = new TH1F(hname.c_str(), titl.c_str(), nx, 0.00, xmax);
  adccalib->Draw(sarg.c_str(), sel.c_str());
  ph->SetStats(0);
  ph->SetLineWidth(2);
  ph->SetTitleOffset(1.7, "y");
  double xlab = 0.62;
  double ylab = 0.80;
  vector<string> slabs;
  {
    ostringstream sslab;
    int prec = ph->GetEntries()>1.e6 ? 3 : 2;
    sslab.precision(prec);
    sslab << "# entries: " << ph->GetEntries()/1.e6 << " #times 10^{6}";
    slabs.push_back(sslab.str());
  }
  {
    ostringstream sslab;
    double mean = ph->GetMean();
    int prec = mean>0.3&&mean< 1.0 ? 2 : 3;
    sslab.precision(prec);
    sslab << "Mean: " << mean << " mV";
    slabs.push_back(sslab.str());
  }
  {
    int nbin = ph->GetNbinsX();
    double nover = ph->Integral(nbin+1, nbin+1);
    double ntot = ph->GetEntries();
    double fover = nover/ntot;
    double fpover = 100*fover;
    int prec = fpover>0.3&&fpover<1.00 ? 1 : 2;
    ostringstream sslab;
    sslab.precision(prec);
    sslab << "Above " << xmax << " mV: " << fpover << "%";
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
  pcan->Print(fname.c_str());
  double selfrac = ph->GetEntries()/25/16/(4096-66);
  cout << "Too few counts: " << 1 - selfrac << endl;
}
