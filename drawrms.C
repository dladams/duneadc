void drawrms() {
  string sel = "Iteration$!=0&&Iteration$<4095&&Iteration$>64&&calCounts>20";
  cout << "Sel: " << sel << endl;
  TH1* ph = new TH1F("hr", "ADC bin resolution distribution (64 < ADC < 4095);Bin sigma [mV];# channels [/(0.01 mV]", 100, 0.00, 1.00);
  adccalib->Draw("calRmss>>hr", sel.c_str());
  ph->SetStats(0);
  ph->SetLineWidth(2);
  TCanvas* pcan = new TCanvas;
  pcan->SetRightMargin(0.05);
  pcan->SetLeftMargin(0.13);
  ph->SetTitleOffset(1.7, "y");
  pcan->cd();
  ph->Draw();
  {
    ostringstream ssmean;
    ssmean.precision(2);
    ssmean << "Mean = " << ph->GetMean() << " mV";
    string smean = ssmean.str();
    TLatex* ptgain = new TLatex(0.62, 0.80, smean.c_str());
    ptgain->SetNDC();
    ptgain->SetTextFont(42);
    ptgain->Draw();
  }
  {
    int nbin = ph->GetNbinsX();
    //double nover = ph->GetBinContent(ph->GetNbinsX()+1);
    double nover = ph->Integral(nbin+1, nbin+1);
    double ntot = ph->GetEntries();
    double fover = nover/ntot;
    double fpover = 100*fover;
    ostringstream ssmean;
    ssmean.precision(2);
    if ( fpover < 0.9 ) ssmean.precision(1);
    ssmean << "Above 1.0 mV: " << fpover << "%";
    string smean = ssmean.str();
    TLatex* ptgain = new TLatex(0.62, 0.73, smean.c_str());
    ptgain->SetNDC();
    ptgain->SetTextFont(42);
    ptgain->Draw();
  }
  {
    int nbin = ph->GetNbinsX();
    double nover = ph->Integral(70, nbin+1);
    double ntot = ph->GetEntries();
    double fover = nover/ntot;
    double fpover = 100*fover;
    ostringstream ssmean;
    ssmean.precision(2);
    ssmean << "Above 0.7 mV: " << fpover << "%";
    string smean = ssmean.str();
    TLatex* ptgain = new TLatex(0.62, 0.66, smean.c_str());
    ptgain->SetNDC();
    ptgain->SetTextFont(42);
    ptgain->Draw();
  }
  pcan->Print("rmslow.png");
  double selfrac = ph->GetEntries()/25/16/(4096-66);
  cout << "Too few counts: " << 1 - selfrac << endl;
}
