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

//**********************************************************************

// Fetch the full list of good DUNE17-cold datasets.

vector<string> dune17cDatasets() {
  ifstream fin("DUNE17-cold.txt");
  vector<string> dss;
  while ( fin ) {
    if ( fin.eof() ) break;
    string ds;
    fin >> ds;
    if ( ds.size() == 0 ) continue;
    dss.push_back(ds);
  }
  return dss;
}

//**********************************************************************

// Process a given list of datasets.
// Not really specific to DUNE17-cold except the AdcChipAnalyzer parameters
// are hardwired.

void filldbd17c(const vector<string>& ssams, Index chan1=0, Index nchan =16, bool fill =true) {
  bool fillCalTree = fill;
  string datasetCalib = "";
  unsigned int nv = 90;
  double vmin = -200.0;
  double vmax = 1600.0;
  double vresmax = 1.0;
  bool dropTails = false;
  bool fillPerfTree = fill;
  for ( string ssam : ssams ) {
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

//**********************************************************************

// Special actions specified by name:
//                 count - Show number of datasets
//                  list - List all datasets
//   DUNE17-cold_chipXXX - Process that dataset
void filldbd17c(string sact) {
  string myname = "filldbd17c: ";
  if ( sact == "count" ) {
    cout << "DUNE17-cold dataset count: " << dune17cDatasets().size() << endl;
  } else if ( sact == "list" ) {
    vector<string> dss = dune17cDatasets();
    for ( string ds : dss ) cout << ds << endl;
  } else if ( sact.substr(0,16) == "DUNE17-cold_chip" ) {
    vector<string> dss(1, sact);
    filldbd17c(dss, 0, 16, true);
  } else {
    cout << myname << "ERROR: Unknown command: " << sact << endl;
 }
}

//**********************************************************************

// Process a range of chip IDs.
// Now (July 7) obsolete as the suffixes are no longer being updated.

void filldbd17c(Index chip1, Index nchip, Index chan1=0, Index nchan =16, bool fill =true) {
  const string myname = "filldb17c: ";
  if ( chan1 + nchan > 16 ) {
    cout << myname << "ERROR: Invalid channel range." << endl;
    return;
  }
  vector<string> ssams;
  for ( Index chip=chip1; chip<chip1+nchip; ++chip ) {
    string dataset = "DUNE17-cold";
    ostringstream sssam;
    sssam << dataset << "_chip" << chip;
    string ssambase = sssam.str();
    vector<string> sufs(1);
    if ( chip ==  2 ) { sufs[0] = "0705"; }  // 0702 test is bad
    if ( chip ==  3 ) { sufs[0] = "0702T16"; }  // 0702T15 test is bad
    if ( chip == 12 ) { sufs[0] = "0705T1142"; }  // 0701 tests and earlier 0705 test are bad
    if ( chip == 29 ) { sufs[0] = "0630"; sufs.push_back("0701"); sufs.push_back("0706"); }
    if ( chip == 30 ) { sufs[0] = "0703"; }  // 0630 is bad
    if ( chip == 61 ) { sufs[0] = "0626"; sufs.push_back("0628"); }
    if ( chip == 63 ) { sufs[0] = "0628T17"; sufs.push_back("0628T21"); sufs.push_back("0628T22"); }
    if ( chip == 65 ) { sufs[0] = "0629"; }  // Four samples from 0628 are bad
    if ( chip == 67 ) { sufs[0] = "0629T10"; sufs.push_back("0629T14"); }
    if ( chip == 71 ) { sufs[0] = "0628T12"; sufs.push_back("0628T17"); }
    if ( chip == 75 ) { sufs.clear(); }  // 0628 two tests look bad. 0629 ADC looks bad.
    if ( chip == 92 ) { sufs[0] = "0630T13"; sufs.push_back("0630T17"); }
    for ( string suf : sufs ) {
      string ssam = ssambase;
      if ( suf.size() ) ssam += "_" + suf;
      ssams.push_back(ssam);
    }
  }
  filldbd17c(ssams, chan1, nchan, fill);
}

//**********************************************************************
