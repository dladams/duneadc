// drawNSigma.cxx

#include "histNSigma.h"
#include "TCanvas.h"
#include "TH1.h"

void drawNSigma(TH1* ph, double nsig, int nrange) {
  TH1* ph0 = histNSigma(ph, 0.0, nrange);
  TH1* phn = histNSigma(ph, nsig, nrange);
  ph0->SetFillStyle(0);
  ph0->SetFillColor(0);
  ph0->SetLineWidth(2);
  TCanvas* pcan = new TCanvas;
  ph0->Draw();
  phn->Draw("same");
  std::string fname = ph0->GetTitle();
  for ( unsigned int ich=0; ich<fname.size(); ++ich ) {
    if ( fname[ich] == ' ' ) fname[ich] = '_';
  }
  fname += ".png";
  pcan->Print(fname.c_str());
}
