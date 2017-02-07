void drawoffset() {
  TH1* ph = new TH1F("ho", "V_{in}:ADC fit offset;Offset [mV];# channels [/(5 mV]", 70, -50, 300);
  adccalib->Draw("offset>>ho");
  ph->SetStats(0);
  ph->SetLineWidth(2);
  TCanvas* pcan = new TCanvas;
  pcan->SetRightMargin(0.05);
  pcan->cd();
  ph->Draw();
  pcan->Print("offset.png");
}
