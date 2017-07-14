// processDataset.C

// David Adams
// July 2017

void processDataset(string ssam, double vresmax=1.0, Index chan1=0, Index nchan =16, bool fill =true) {
  string myname = "processDataset: ";
  bool fillCalTree = fill;
  string datasetCalib = "";
  unsigned int nv = 90;
  double vmin = -200.0;
  double vmax = 1600.0;
  bool dropTails = false;
  bool fillPerfTree = fill;
  bool savePlots = true;
  cout << myname << "Processing sample " << ssam << endl;
  TDatime dt1;
  dt1.Print();
  AdcChipAnalyzer chipper(ssam, chan1, nchan, datasetCalib, fillCalTree, vmin, vmax, nv, vresmax, dropTails, fillPerfTree);
  chipper.draw("rawv", savePlots);
  chipper.draw("diff", savePlots);
  chipper.draw("perf", savePlots);
  TDatime dt2;
  dt2.Print();
  cout << "Elapsed time: " << (dt2.Convert() - dt1.Convert())/60.0 << " minutes" << endl;
}

