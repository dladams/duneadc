// filldb03a.C
//
// David Adams
// May 2017
//
// Performance tre filler for dataset 201703a, the survey of
// 77 chips D01 - D79.

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

void filldb03a(Index chip1=1, Index nchip =77, int chan1=0, int nchan =16) {
  vector<string> schips;
  Index chip = chip1;
  Index count = 0;
  for ( Index chip=chip1; schips.size() < nchip; ++chip ) {
    if ( chip < 1 ) continue;
    if ( chip == 2 ) continue;
    if ( chip == 6 ) continue;
    if ( chip > 79 ) break;
    ostringstream sschip;
    sschip << "D";
    if ( chip < 10 ) sschip << "0";
    sschip << chip;
    schips.push_back(sschip.str());
  }
  string datasetCalib;
  bool fillCalTree = true;
  unsigned int nv = 90;
  double vmin = -200.0;
  double vmax = 1600.0;
  double vresmax = 1.0;
  bool dropTails = false;
  bool fillPerfTree = true;
  cout << "Dataset count: " << schips.size() << endl;
  for ( string schip : schips ) {
    ifstream stopfile("stop");
    if ( stopfile ) {
      cout << endl;
      cout << "Execution halted by stop file." << endl;
      break;
    }
    string dataset = "201703a";
    string dschip = dataset + "_" + schip;
    cout << endl;
    cout << "*************  " << dschip << "  *****************" << endl;
    TDatime dt1;
    dt1.Print();
    AdcChipAnalyzer chipper(dschip, chan1, nchan, datasetCalib, fillCalTree, vmin, vmax, nv, vresmax, dropTails, fillPerfTree);
    TDatime dt2;
    dt2.Print();
    cout << "Elapsed time: " << (dt2.Convert() - dt1.Convert())/60.0 << " minutes" << endl;
  }
}
