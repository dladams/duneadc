void drawChipBounds(TH1* phin, bool off =false, bool dolabs =false, bool drawhist =false) {
cout << "XXXXXXXXXXXXXXXXXXXXXXX" << endl;
  vector<unsigned int> goodchips = { 0, 4, 8, 10, 16, 24, 31, 18, 15, 33, 30, 5};
  TH1* ph = nullptr;
  if ( drawhist ) {
    ph = (TH1*) phin->Clone();
    if ( dolabs ) {
      ph->GetXaxis()->SetLabelColor(0);
      ph->GetXaxis()->SetTitle("Chip");
      ph->GetXaxis()->SetTickSize(0);
    }
    ph->DrawCopy();
  }
  // Find vertical limits of the histogram display.
  double ymin = phin->GetMinimum();
  double ymax = phin->GetMaximum();
  if ( ymax == ymin ) {
    ymin = phin->GetYaxis()->GetXmin();
    ymax = phin->GetYaxis()->GetXmax();
  }
  for ( int ichp=0; ichp<25; ++ichp ) {
    // Left boundary of this chip.
    double x = 16*ichp - 0.5*off;
    // Find the chip number.
    unsigned int chip = ichp;
    TTree* ptree = dynamic_cast<TTree*>(gDirectory->Get("adccalib"));
    if ( ptree == nullptr ) {
      cout << "drawChipBounds: Unable to find tree adccalib." << endl;
    } else {
      AdcChannelCalibration* pacc = new AdcChannelCalibration;
      char* paccold = ptree->GetBranch("cal")->GetAddress();
      ptree->GetBranch("cal")->SetAddress(&pacc);
      ptree->GetEntry(16*ichp);
      chip = pacc->chip;
      ptree->GetBranch("cal")->SetAddress(paccold);
    }
    // Display shaded box for bad channels.
    if ( find(goodchips.begin(), goodchips.end(), chip) == goodchips.end() ) {
      TBox* pbox = new TBox(x, ymin, x+16, 0.997*ymax);
      pbox->SetLineStyle(0);
      pbox->SetFillColor(18);
      //pbox->SetFillStyle(4000);
      pbox->Draw();
    }
    // Draw lines between chips.
    if ( ichp != 0 ) {
      TLine* pline = new TLine(x, ymin, x, ymax);
      pline->SetLineStyle(3);
      pline->Draw();
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
  if ( ph != nullptr ) ph->DrawCopy("same");
}

