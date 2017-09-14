// filldb06cots.C
//
// David Adams
// June 2017
//
// Performance tree filler for DUNE17-cold--col test data for protoDUNE ADCs.

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "AdcChipAnalyzer.h"

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;
using std::vector;

// dataset:
//   201706_cots   - Original COTS cold
//   201706_cotsw  - Original COTS warm
//   201706_cotsb  - First (partial) update cold
//   201706_cotsbw - First (partial) update warm
//   201708_cotsBB - Aug data
void filldb06cots(string dataset, Index chip1, Index nchip, Index chan1=0, Index nchan =16,
                  SampleIndex isam0 =0, SampleIndex nsam =0, bool fill =true) {
  const string myname = "filldb06cots: ";
  if ( chan1 + nchan > 16 ) {
    cout << myname << "ERROR: Invalid channel range." << endl;
    return;
  }
  string datasetCalib = "";
  bool fillCalTree = fill;
  unsigned int nv = 135;
  double vmin = -200.0;
  double vmax = 2500.0;
  double vresmax = 2.0;   // Changed from 1.0 06aug2017
  bool dropTails = false;
  bool fillPerfTree = fill;
  for ( Index chip=chip1; chip<chip1+nchip; ++chip ) {
    ostringstream sssam;
    string prefix;
    if ( dataset == "201708_cots" ) prefix = "0";
    sssam << dataset << prefix << chip;
    if ( isam0 > 0 || nsam > 0 ) sssam << "_" << isam0 << "_" << nsam;
    string ssam = sssam.str();
    cout << endl;
    cout << "*************  " << ssam << "  *****************" << endl;
    TDatime dt1;
    dt1.Print();
    AdcChipAnalyzer chipper(ssam, chan1, nchan, datasetCalib, fillCalTree, vmin, vmax, nv, vresmax, dropTails, fillPerfTree);
    bool save = true;
    chipper.draw("rawv", save);
    chipper.draw("diff", save);
    chipper.draw("perf", save);
    TDatime dt2;
    dt2.Print();
    cout << "Elapsed time: " << (dt2.Convert() - dt1.Convert())/60.0 << " minutes" << endl;
  }
}
