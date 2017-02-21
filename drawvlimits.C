void drawvlimits(bool same =false) {
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
    drawChipBounds(ph, true, true, true);
    //ph->DrawCopy();
  }
  adccalib->SetMarkerStyle(7);
  string sarg = "gain*" + sadcmin + "+offset:Entry$";
  adccalib->Draw(sarg.c_str(), "", "same");
  adccalib->SetMarkerStyle(6);
  sarg = "gain*" + sadcmax + "+offset:Entry$";
  adccalib->Draw(sarg.c_str(), "", "same");
  adccalib->SetMarkerStyle(msty);
  if ( pcan != nullptr ) pcan->Print("vlimits.png");
}
