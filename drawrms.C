TH1* makeDrawrmsHist(int adcmin, int adcmax, double xmax, string sstat) {
  ostringstream sstitl;
  string hname = "hr";
  sstitl << "ADC bin resolution (" << adcmin << " < ADC < " << adcmax;
  if ( sstat == "good" ) {
    hname += "good";
  } else {
    hname += "all";
  }
  sstitl << ")";
  int nx = 100;
  sstitl << ";Bin sigma [mV];# ADC bins [/(" << xmax/nx << " mV]";
  string titl = sstitl.str();
  TH1* ph = new TH1F(hname.c_str(), titl.c_str(), nx, 0.00, xmax);
  ph->SetStats(0);
  ph->SetLineWidth(2);
  ph->SetTitleOffset(1.7, "y");
  return ph;
}

void showHist(TH1* ph, string sstat, double xmax) {
  string fname = "rmslow_";
  if ( sstat == "good" ) {
    fname += "good";
  } else {
    fname += "all";
  }
  fname += ".png";
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

void drawrms(string dsname, int adcmin =65, int adcmax=4095, double xmax =1.0, string sstat ="all") {
  const string myname = "drawrms: ";
  string dsfname = dsname + ".txt";
  ifstream dsfile(dsfname.c_str());
  if ( ! dsfile ) {
    cout << myname << "Dastaset file not found: " << dsfname << endl;
    return nullptr;
  }
  vector<string> ssams;
  Index wsam = 0;
  while ( dsfile ) {
    string ssam;
    dsfile >> ssam;
    if ( dsfile.eof() ) break;
    ssams.push_back(ssam);
    Index lsam = ssam.size();
    if ( lsam > wsam ) wsam = lsam;
    cout << ssam << endl;
  }
  cout << myname << "Sample count: " << ssams.size() << endl;
  string dscfile = "calib_" + dsname + ".root";
  AdcCalibrationTree act(dscfile);
  if ( act.size() == 0 ) {
    cout << myname << "Calibration has no entries." << endl;
    return;
  }
  cout << myname << "Calibration has " << act.size() << " entries." << endl;
  Index nsc;
  Index nscGood = 0;
  Index ient = 0;
  TH1* ph = makeDrawrmsHist(adcmin, adcmax, xmax, sstat);
  for ( string ssam : ssams ) {
    for ( Index chan=0; chan<16; ++chan ) {
      ++nsc;
      const AdcTreeChannelCalibration* pcal = act.find(ssam, chan, ient);
      if ( pcal == 0 ) {
        ient = 0;
        pcal = act.find(ssam, chan, ient);
      }
      if ( pcal == nullptr ) {
        cout << myname << "WARNING: Calibration not found for sample " << ssam
             << " channel " << chan << endl;
        continue;
      }
      ++nscGood;
      for ( Index iadc=adcmin; iadc<adcmax; ++iadc ) {
        ph->Fill(pcal->calRms(iadc));
      }
    }
  }
  cout << myname << "Found " << nscGood << " of " << nsc << " sample channels." << endl;
  showHist(ph, sstat, xmax);
}

void drawrms(int adcmin =65, int adcmax=4095, double xmax =1.0, string sstat ="all", bool doLinear =false) {
  ostringstream sssel;
  sssel << "Iteration$>" << adcmin << "&&Iteration$<" << adcmax << "&&calCounts>20";
  if ( sstat == "good" ) {
    sssel << "&&AdcChipStatus::good(chip)";
  }
  string sarg = "calRmss";
  if ( doLinear ) {
    //sstitl << " for linear calib";
    //hname += "lin";
    //fname += "_linear";
    xmax = 10.0;
    sarg = "sqrt(calRmss*calRmss + (calMeans-gain*Iteration$-offset)*(calMeans-gain*Iteration$-offset))";
  }
  string sel = sssel.str();
  cout << "Sel: " << sel << endl;
  TH1* ph = makeDrawrmsHist(adcmin, adcmax, xmax, sstat);
  sarg += ">>";
  sarg += ph->GetName();
  TTree* ptree = dynamic_cast<TTree*>(gDirectory->Get("adccalib"));
  if ( ptree == 0 ) {
    cout << "Tree not found." << endl;
    return;
  }
  ptree->Draw(sarg.c_str(), sel.c_str());
  showHist(ph, sstat, xmax);
}

