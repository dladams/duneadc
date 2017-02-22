void drawChipBounds(TH1* phin, bool off, bool dolabs, bool drawhist, vector<unsigned int> chips) {
  const string myname = "drawChipBounds: ";
  //vector<unsigned int> badchips = { 2, 6, 7, 14, 21, 29, 33, 35, 15, 18, 25, 3, 17};
  AdcChipStatus acs;
  TH1* ph = nullptr;
  string darg = "";
  unsigned int nchp = 25;
  if ( drawhist ) {
    ph = phin;
    nchp = (phin->GetNbinsX()+15)/16;
    //ph = (TH1*) phin->Clone();
    if ( dolabs ) {
      ph->GetXaxis()->SetLabelColor(0);
      ph->GetXaxis()->SetTitle("Chip");
      ph->GetXaxis()->SetTickSize(0);
    }
    TH2* ph2 = dynamic_cast<TH2*>(ph);
    if ( ph2 == nullptr ) {
      ph->DrawCopy();
    } else {
      darg = "colz";
      ph2->Draw("colz");
      double palx1 = 0.941;
      double palx2 = 0.960;
      gPad->Update();
      TPaletteAxis* ppalax = dynamic_cast<TPaletteAxis*>(ph2->GetListOfFunctions()->FindObject("palette"));
      if ( ppalax != nullptr ) {
        ppalax->SetX1NDC(palx1);
        ppalax->SetX2NDC(palx2);
        //ppalax->SetTitleOffset(0.20);
        ph2->GetZaxis()->SetTickSize(0.015);
        ph2->GetZaxis()->SetTitle("Efficiency");
        ph2->GetZaxis()->SetTitleOffset(0.55);
      }
      ph2->Draw("colz");
    }
  }
  // Find horizontal limits of the histogram display.
  double xmin = phin->GetXaxis()->GetXmin();
  double xmax = phin->GetXaxis()->GetXmax();
  // Find vertical limits of the histogram display.
  double ymin = phin->GetYaxis()->GetXmin();
  double ymax = phin->GetYaxis()->GetXmax();
  if ( ymax == ymin || ( ymin == 0.0 && ymax == 1.0 ) ) {
    ymin = phin->GetMinimum();
    ymax = phin->GetMaximum();
  }
  vector<TLine*> lines;
  for ( int ichp=0; ichp<nchp; ++ichp ) {
    // Left boundary of this chip.
    double x = 16*ichp - 0.5*off;
    // Find the chip number.
    unsigned int chip = ichp;
    if ( chips.size() > ichp ) {
      chip = chips[ichp];
    } else {
      TTree* ptree = dynamic_cast<TTree*>(gDirectory->Get("adccalib"));
      if ( ptree == nullptr ) {
        ptree = dynamic_cast<TTree*>(gDirectory->Get("adcperf"));
        if ( ptree == nullptr ) {
          cout << "drawChipBounds: Unable to find tree adccalib or adcperf." << endl;
        } else {
          AdcVoltagePerformance* pavp = new AdcVoltagePerformance;
          char* pavpold = ptree->GetBranch("perf")->GetAddress();
          ptree->GetBranch("perf")->SetAddress(&pavp);
          ptree->GetEntry(16*ichp);
          chip = pavp->chip;
          ptree->GetBranch("perf")->SetAddress(pavpold);
        }
      } else {
        AdcChannelCalibration* pacc = new AdcChannelCalibration;
        char* paccold = ptree->GetBranch("cal")->GetAddress();
        ptree->GetBranch("cal")->SetAddress(&pacc);
        ptree->GetEntry(16*ichp);
        chip = pacc->chip;
        ptree->GetBranch("cal")->SetAddress(paccold);
      }
    }
cout << myname << "chip[" << ichp << "] = " << chip << endl;
    // Display shaded box for bad channels.
    if ( acs.isBad(chip) ) {
      TBox* pbox = new TBox(x, ymin, x+16, 0.997*ymax);
      pbox->SetLineStyle(0);
      pbox->SetFillColor(17);
      pbox->Draw();
    }
    // Draw lines between chips.
    if ( ichp != 0 ) {
      TLine* pline = new TLine(x, ymin, x, ymax);
      pline->SetLineStyle(3);
      pline->Draw();
      lines.push_back(pline);
    }
    // Show chip numbers.
    if ( dolabs ) {
      double xlab = x + 8;
      double ylab = ymin - 0.03*(ymax-ymin);
      ostringstream sslab;
      sslab << chip;
      string slab = sslab.str();
      TLatex* ptxt = new TLatex(xlab, ylab, slab.c_str());
      ptxt->SetTextFont(42);
      ptxt->SetTextAlign(22);
      ptxt->SetTextSize(0.04);
      //ptxt->SetBBoxCenterX(xlab);
      //ptxt->SetBBoxCenterY(ylab);
      //ptxt->SetNDC(false);
      ptxt->Draw();
    }
    // Create histogram of bad channels.
  }
  // Add line for eff = 0;
  if ( ymin < 0.0 && ymax > 0.0 ) {
    TLine* pline = new TLine(xmin, 0.0, xmax, 0.0);
    lines.push_back(pline);
  }
  darg += " same";
  if ( ph != nullptr ) ph->Draw(darg.c_str());
  // Redraw lines.
  for ( TLine* pline : lines ) pline->Draw();
  // Redraw histogram axis and grid.
  if ( ph != nullptr ) ph->Draw("axis same");
}

