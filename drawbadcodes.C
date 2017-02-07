void drawbadcodes() {
  string sel = "Iteration$!=0&&Iteration$<4095&&Iteration$>64&&calCounts>20&&calRmss>0.8";
  cout << "Sel: " << sel << endl;
  TH1* ph = new TH1F("hb", "ADC bad code distribution (64 < ADC < 4095, RMS > 0.8 mV);(ADC code)%64;# bins [/code]", 64, 0, 64);
  adccalib->Draw("Iteration$%64>>hb", sel.c_str());
  ph->SetStats(0);
  ph->SetLineWidth(2);
  TCanvas* pcan = new TCanvas;
  pcan->SetLeftMargin(0.12);
  ph->SetTitleOffset(1.3, "y");
  pcan->SetRightMargin(0.05);
  pcan->cd();
  ph->Draw();
  pcan->Print("badbinperchip.png");
  double selfrac = ph->GetEntries()/25/16/(4096-66);
  cout << "Too few counts: " << 1 - selfrac << endl;
  pcan->Print("badcodes.png");
}
