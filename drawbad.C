void drawbad() {
  string sel = "Iteration$!=0&&Iteration$<4095&&Iteration$>64&&calCounts>20&&calRmss>0.8";
  cout << "Sel: " << sel << endl;
  TH1* ph = new TH1F("hb", "ADC bad bin distribution (64 < ADC < 4095, RMS > 0.8 mV);Chip index;# channels [/chip]", 25, 0, 25);
  adccalib->Draw("Entry$>>hb", sel.c_str());
  ph->SetStats(0);
  ph->SetLineWidth(2);
  TCanvas* pcan = new TCanvas;
  pcan->SetRightMargin(0.05);
  pcan->cd();
  ph->Draw();
  pcan->Print("badbinperchip.png");
  double selfrac = ph->GetEntries()/25/16/(4096-66);
  cout << "Too few counts: " << 1 - selfrac << endl;
  TLine* pline = new TLine(0, 40.3, 25, 40.3);
  pline->SetLineStyle(3);
  pline->Draw();
  TLatex* ptlab = new TLatex(0.14, 0.19, "1%");
  ptlab->SetNDC();
  ptlab->SetTextFont(42);
  ptlab->Draw();
  pcan->Print("badbin.png");
}
