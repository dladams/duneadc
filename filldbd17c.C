// filldbd17c.C
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

void filldbd17c(Index chip1, Index nchip, Index chan1=0, Index nchan =16,
                SampleIndex isam0 =0, SampleIndex nsam =0, bool fill =true) {
  const string myname = "filldb17c: ";
  if ( chan1 + nchan > 16 ) {
    cout << myname << "ERROR: Invalid channel range." << endl;
    return;
  }
  string datasetCalib = "";
  bool fillCalTree = fill;
  unsigned int nv = 90;
  double vmin = -200.0;
  double vmax = 1600.0;
  double vresmax = 1.0;
  bool dropTails = false;
  bool fillPerfTree = fill;
  for ( Index chip=chip1; chip<chip1+nchip; ++chip ) {
    string dataset = "DUNE17-cold";
    ostringstream sssam;
    sssam << dataset << "_chip" << chip;
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
