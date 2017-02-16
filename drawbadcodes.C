void drawbadcodes(double rmsmax) {
  ostringstream sslim;
  sslim << rmsmax;
  string slim = sslim.str();
  string sadcmin = "64";
  sadcmin = "512";
  string sel = "Iteration$!=0&&Iteration$<4095&&Iteration$>" + sadcmin + "&&calCounts>20&&calRmss>" + slim;
  cout << "Sel: " << sel << endl;
  string stitl = "ADC bad code distribution (" + sadcmin + " < ADC < 4095, RMS > " + slim + " mV);(ADC code)%64;# bins [/code]";
  TH1* ph = new TH1F("hb", stitl.c_str(), 64, 0, 64);
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
  string fname = "badcodes_" + slim;
  for ( unsigned int ich=0; ich<fname.size(); ++ich ) {
    if ( fname[ich] == '.' ) fname[ich] = 'p';
  }
  fname += ".png";
  pcan->Print(fname.c_str());
}
