void drawveff(bool skipbad =false) {
  string myname = "drawveff: ";
  AdcChipStatus acs;
  TTree* ptree = dynamic_cast<TTree*>(gDirectory->Get("adcperf"));
  if ( ptree == nullptr ) {
    cout << myname << "Performance tree not found." << endl;
    gDirectory->ls();
    return;
  }
  AdcVoltagePerformance* pavp = nullptr;
  ptree->SetBranchAddress("perf", &pavp);
  ptree->GetEntry(0);
  unsigned int nv = pavp->nv;
  float vmin = pavp->vmin;
  float vmax = pavp->vmax;
  float vuncmax = pavp->vuncmax;
  if ( nv == 0 ) {
    cout << myname << "First performance object has no voltages." << endl;
    return;
  }
  cout << myname << "# voltages: " << nv << endl;
  cout << myname << "Voltage range: (" << vmin << ", " << vmax << ") mV" << endl;
  cout << myname << "Voltage limit: " << vuncmax << " mV" << endl;
  unsigned int nent = ptree->GetEntries();
  cout << myname << "# tree entries: " << nent << endl;
  // Create vector of entries to display.
  vector<unsigned int> ents;
  unsigned chip = 99999;
  vector<unsigned int> chips;
  for ( unsigned int ient=0; ient<nent; ++ient ) {
    ptree->GetEntry(ient);
    if ( pavp->nv != nv ) {
      cout << myname << "Entry " << ient << " is inconsistent." << endl;
      return;
    }
    unsigned int newchip = pavp->chip;
    if ( skipbad ) {
      if ( acs.isBad(newchip) ) {
        cout << myname << "Skipping bad chip " << newchip << ", channel " << pavp->chan << endl;
        continue;
      }
    }
    ents.push_back(ient);
    if ( chip != newchip ) {
      chip = newchip;
      chips.push_back(chip);
    }
  }
  cout << myname << "# selected channels: " << ents.size() << "/" << nent << endl;
  // Create histogram.
  unsigned int ncha = ents.size();
  ostringstream sstitl;
  sstitl << "Input voltage efficiency for RMS(V_{in}) < " << vuncmax << " mV; Channel; V_{in} [mV]";
  string stitl = sstitl.str();
  TH2* ph = new TH2F("hveffsum", stitl.c_str(), ncha, 0, ncha, nv, vmin, vmax);
  ph->SetStats(0);
  ph->SetContour(20);
  ph->SetStats(0);
  ph->GetYaxis()->SetTickLength(0.01);
  // Fill histogram.
  int xbin = 0;
  for ( unsigned int ient : ents ) {
    ++xbin;
    ptree->GetEntry(ient);
    //cout << myname << "Entry: " << ient << "; chip " << pavp->chip << ", channel " << pavp->chan << endl;
    for ( unsigned int iv=0; iv<nv; ++iv ) {
      unsigned int bin = ph->GetBin(xbin, iv+1);
      double eff = pavp->eff(iv);
      //cout << "   " << ient << ", " << iv << ": " << eff << endl;
      if ( eff > 0.50 ) {
        ph->SetBinContent(bin, pavp->eff(iv));
      }
    }
  }
  // Draw histogram.
  palette(41);
  string cname = "veffall";
  if ( skipbad ) cname = "veffgood";
  TCanvas* pcan = new TCanvas(cname.c_str(), cname.c_str(), 1500, 500);
  pcan->SetGridy();
  pcan->SetRightMargin(0.06);
  pcan->SetLeftMargin(0.05);
  ph->SetTitleOffset(0.7, "y");
  drawChipBounds(ph, false, true, true, chips);
  //ph->DrawCopy("colz");
  if ( 1 ) {
    TDirectory* psavedir = gDirectory;
    TFile* pfile = TFile::Open("calib_201701.root");
    drawvlimits(true, chips);
    delete pfile;
    psavedir->cd();
  }
  string fname = pcan->GetName();
  fname +=".png";
  pcan->Print(fname.c_str());
}
