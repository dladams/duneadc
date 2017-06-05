// filldb03b.C
//
// David Adams
// May 2017
//
// Performance tree filler for dataset 201703b, repeated testing
// of chip D02 kept cold.

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

void filldb03b(Index itim1=0, Index ntim=18, Index chan1=0, Index nchan =16) {
  const string myname = "filldb03b: ";
  string schip = "D02";
  vector<string> stimes = {"1a", "2a", "3a", "3b", "4a", "4b", "5a", "5b",
                           "6a", "6b", "7a", "7b", "8a", "8b", "8c", "9a", "9b", "9c"};
  Index itim2 = itim1 + ntim;
  if ( itim2 > stimes.size() ) {
    cout << myname << "ERROR: Too many times requested." << endl;
    return;
  }
  if ( chan1 + nchan > 16 ) {
    cout << myname << "ERROR: Invalid channel range." << endl;
    return;
  }
  string datasetCalib = "calib_201703b_0602.root";
  bool fillCalTree = true;
  unsigned int nv = 90;
  double vmin = -200.0;
  double vmax = 1600.0;
  double vresmax = 1.0;
  bool dropTails = false;
  bool fillPerfTree = true;
  cout << "Dataset count: " << stimes.size() << endl;
  for ( Index itim=itim1; itim<itim2; ++itim ) {
    string stime = stimes[itim];
    ifstream stopfile("stop");
    if ( stopfile ) {
      cout << endl;
      cout << "Execution halted by stop file." << endl;
      break;
    }
    string dataset = "201703b";
    string dschip = dataset + "_" + schip + "_" + stime;
    cout << endl;
    cout << "*************  " << dschip << "  *****************" << endl;
    TDatime dt1;
    dt1.Print();
    AdcChipAnalyzer chipper(dschip, chan1, nchan, datasetCalib, fillCalTree, vmin, vmax, nv, vresmax, dropTails, fillPerfTree);
    bool save = true;
    chipper.draw("rawv", save);
    chipper.draw("diff", save);
    chipper.draw("difn", save);
    chipper.draw("perf", save);
    TDatime dt2;
    dt2.Print();
    cout << "Elapsed time: " << (dt2.Convert() - dt1.Convert())/60.0 << " minutes" << endl;
  }
}
