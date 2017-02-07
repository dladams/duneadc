void drawgain() {
  TH1* phg = new TH1F("hg", "V_{in}:ADC fit gain in mV/(ADC count);Gain [mV];# channels [/(0.2 mV]", 40, 0.25, 0.45);
  adccalib->Draw("gain>>hg");
  phg->SetStats(0);
  phg->SetLineWidth(2);
  TCanvas* pcan = new TCanvas;
  pcan->SetRightMargin(0.05);
  pcan->cd();
  phg->Draw();
  {
    ostringstream ssmean;
    ssmean.precision(3);
    ssmean << "Mean = " << phg->GetMean() << " mV";
    string smean = ssmean.str();
    TLatex* ptgain = new TLatex(0.65, 0.80, smean.c_str());
    ptgain->SetNDC();
    ptgain->SetTextFont(42);
    ptgain->Draw();
  }
  
  {
    ostringstream ssmean;
    ssmean.precision(3);
    ssmean << " RMS = " << phg->GetRMS() << " mV";
    string smean = ssmean.str();
    TLatex* ptgain = new TLatex(0.65, 0.73, smean.c_str());
    ptgain->SetNDC();
    ptgain->SetTextFont(42);
    ptgain->Draw();
  }
  pcan->Print("gain.png");
}
