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

void filldb03(string datagroup ="201703a", int chip1=0, int chip2=80) {
  vector<string> schips;
  for ( int chip=chip1; chip<chip2; ++chip ) {
    if ( chip < 1 ) continue;
    if ( chip == 2 ) continue;
    if ( chip == 6 ) continue;
    if ( chip > 79 ) continue;
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
  bool fillPerfTree = true;
  cout << "Dataset count: " << schips.size() << endl;
  for ( string schip : schips ) {
    ifstream stopfile("stop");
    if ( stopfile ) {
      cout << endl;
      cout << "Execution halted by stop file." << endl;
      break;
    }
    string dataset = datagroup + "_" + schip;
    cout << endl;
    cout << "*************  " << dataset << "  *****************" << endl;
    TDatime dt1;
    dt1.Print();
    AdcChipAnalyzer chipper(dataset, 0, 16, datasetCalib, fillCalTree, vmin, vmax, nv, vresmax, fillPerfTree);
    TDatime dt2;
    dt2.Print();
    cout << "Elapsed time: " << (dt2.Convert() - dt1.Convert())/60.0 << " minutes" << endl;
  }
}
