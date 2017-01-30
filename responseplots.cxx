// responseplots.cxx

#include <iostream>
#include <sstream>
#include "AdcHist.h"
#include "TCanvas.h"
#include "TROOT.h"

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;
typedef unsigned int Index;

void responseplots(string ssam ="p1", Index icha1 =0, Index ncha =0) {
  string myname = "responseplots: ";
  if ( ncha == 0 ) ncha = 16 - icha1;
  TCanvas* pcan = nullptr;
  if ( gROOT->IsBatch() && ncha > 4 ) {
    pcan = new TCanvas("mycan", "mycan", 2800, 2000);
  } else {
    pcan = new TCanvas();
  }
  Index npad = 0;
  if ( ncha > 4 ) {
    pcan->Divide(4, 4);
    npad = 16;
  } else if ( ncha > 1 ) {
    pcan->Divide(2, 2, 0.01, 0.01);
    npad = 4;
  }
  for ( Index icha=icha1; icha<icha1+ncha; ++icha ) {
    if ( npad ) pcan->cd(icha%npad+1);
    gPad->SetGridy();
    AdcHist adchist(ssam, icha);
    if ( adchist.phc == nullptr ) {
      cout << "No response plot for " << ssam << " channel " << icha << endl;
      continue;
    }
    adchist.phc->Draw("colz");
  }
  string schan;
  if ( npad < 16 ) {
    ostringstream sschan;
    sschan << "_";
    if ( icha1 < 10 ) sschan << "0";
    sschan << icha1;
    if ( ncha ) {
      sschan << "_n" << ncha;
    }
    schan = sschan.str();
  }
  string fname = "response_" + ssam + schan + ".png";
  pcan->Print(fname.c_str());
}

