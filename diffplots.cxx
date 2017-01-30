// diffplots.cxx

#include <iostream>
#include <sstream>
#include "AdcHist.h"
#include "TCanvas.h"

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;
typedef unsigned int Index;

void diffplots(string ssam ="p1", Index icha1 =0, Index ncha =0) {
  string myname = "diffplots: ";
  if ( ncha == 0 ) ncha = 16 - icha1;
  Index cmin = 0;
  Index cmax = 15;
  TCanvas* pcan = new TCanvas;
  pcan->SetGridy();
  for ( Index icha=icha1; icha<icha1+ncha; ++icha ) {
    AdcHist adchist(ssam, icha);
    if ( adchist.phd == nullptr ) {
      cout << "No diff plot for " << ssam << " channel " << icha << endl;
      continue;
    }
    adchist.phd->Draw("colz");
    ostringstream sschan;
    if ( icha < 10 ) sschan << "0";
    sschan << icha;
    string schan = sschan.str();
    string fname = "diff_" + ssam + "_" + schan + ".png";
    pcan->Print(fname.c_str());
  }
}

