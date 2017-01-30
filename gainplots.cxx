// gainplots.cxx

#include <iostream>
#include <sstream>
#include "AdcHist.h"
#include "TCanvas.h"

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;
typedef unsigned int Index;

void gainplots(string ssam ="p1", Index icha1 =0, Index ncha =0) {
  string myname = "gainplots: ";
  if ( ncha == 0 ) ncha = 16 - icha1;
  Index cmin = 0;
  Index cmax = 15;
  TCanvas* pcan = new TCanvas;
  pcan->SetGridy();
  for ( Index icha=icha1; icha<icha1+ncha; ++icha ) {
    AdcHist adchist(ssam, icha);
    if ( adchist.phg == nullptr ) {
      cout << "No gain plot for " << ssam << " channel " << icha << endl;
      continue;
    }
    adchist.phg->Draw("colz");
    ostringstream sschan;
    if ( icha < 10 ) sschan << "0";
    sschan << icha;
    string schan = sschan.str();
    string fname = "gain_" + ssam + "_" + schan + ".png";
    pcan->Print(fname.c_str());
  }
}

