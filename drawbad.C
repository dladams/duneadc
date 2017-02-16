void drawbad() {
  vector<string> slims = {"0.8", "1.2"};
  vector<string> hnams = {"hb0p8", "hb1p2"};
  vector<int> cols = {4, 2};
  vector<int> lwids = {2, 1};
  vector<int> fstys = {0, 1001};
  vector<TH1*> hists;
  double ymin = 0.0;
  double ymax = 500.0;
  for ( unsigned int ih=0; ih<slims.size(); ++ih ) {
    cout << endl;
    string slim = slims[ih];
    string hnam = hnams[ih];
    int col = cols[ih];
    int lwid = lwids[ih];
    int fsty = fstys[ih];
    string sel = "Iteration$!=0&&Iteration$<4095&&Iteration$>64&&calCounts>20&&calRmss>" + slim;
    cout << "Sel: " << sel << endl;
    string sarg = "Entry$>>" + hnam;
    string stitl = "ADC bad bin distribution (64 < ADC < 4095);Channel;# bad bins [/channel]";
    TH1* ph = new TH1F(hnam.c_str(), stitl.c_str(), 400, 0, 400);
    hists.push_back(ph);
    adccalib->Draw(sarg.c_str(), sel.c_str(), "goff");
    ph->SetStats(0);
    ph->SetLineWidth(lwid);
    ph->SetLineColor(col);
    ph->SetFillColor(col);
    ph->SetFillStyle(fsty);
    ph->GetYaxis()->SetTickLength(0.01);
    ph->SetTitleOffset(0.7, "y");
    ph->GetYaxis()->SetRangeUser(ymin, ymax);
    //if ( ih == 0 ) drawChipBounds(ph, true, true, true);
    //else ph->Draw("same");
    double selfrac = ph->GetEntries()/25/16/(4096-66);
    cout << "Too few counts: " << 1 - selfrac << endl;
  }
  TCanvas* pcan = new TCanvas("cbad", "cbad", 1500, 500);
  pcan->SetRightMargin(0.02);
  pcan->SetLeftMargin(0.05);
  pcan->cd();
  drawChipBounds(hists[0], true, true, true);
  for ( unsigned int ih=1; ih<slims.size(); ++ih ) hists[ih]->Draw("same");
  TLine* pline = new TLine(0, 5*40.3, 400, 5*40.3);
  pline->SetLineStyle(3);
  pline->Draw();
  TLatex* ptlab = new TLatex(0.06, 0.435, "5%");
  ptlab->SetNDC();
  ptlab->SetTextSize(0.04);
  ptlab->SetTextFont(42);
  ptlab->Draw();
  pline = new TLine(0, 40.3, 400, 40.3);
  pline->SetLineStyle(3);
  pline->Draw();
  ptlab = new TLatex(0.06, 0.175, "1%");
  ptlab->SetNDC();
  ptlab->SetTextSize(0.04);
  ptlab->SetTextFont(42);
  ptlab->Draw();
  TLegend* pleg = new TLegend(0.85, 0.76, 0.93, 0.88);
  for ( unsigned int ih=0; ih<slims.size(); ++ih ) {
    string slab = "RMS > " + slims[ih];
    pleg->AddEntry(hists[ih], slab.c_str(), "f");
  }
  pleg->SetBorderSize(0);
  pleg->SetFillStyle(0);
  pleg->Draw();
  pcan->Print("badbin.png");
}
