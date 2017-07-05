// filldbd17c.C
//
// David Adams
// June 2017
//
// Performance tree filler for DUNE17-cold, cold test data for protoDUNE ADCs.

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
    vector<string> sufs(1);
    if ( chip == 29 ) { sufs[0] = "0630"; sufs.push_back("0701"); }
    if ( chip == 30 ) { sufs[0] = "0703"; }  // 0630 is bad
    if ( chip == 61 ) { sufs[0] = "0626"; sufs.push_back("0628"); }
    if ( chip == 63 ) { sufs[0] = "0628T17"; sufs.push_back("0628T21"); sufs.push_back("0628T22"); }
    string ssambase = sssam.str();
    for ( string suf : sufs ) {
      string ssam = ssambase;
      if ( suf.size() ) ssam += "_" + suf;
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
}
