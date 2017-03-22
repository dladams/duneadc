void filldb(string datagroup ="201701b") {
  vector<string> schips = {"00", "02", "03", "04", "05", "06", "07", "08",
                           "10", "14", "15", "16", "17", "18",
                           "21", "22", "24", "25", "26", "29",
                           "30", "31", "32", "33", "35" };
  vector<string> schipsb = {"00", "02", "03", "04", "05", "07", "08",
                            "10", "14", "15", "16", "17", "18",
                            "21", "22", "24", "25", "26", "29",
                            "30", "31", "32", "33", "35" };
  string datasetCalib;
  bool fillCalTree = true;
  unsigned int nv = 90;
  double vmin = -200.0;
  double vmax = 1600.0;
  double vresmax = 1.0;
  bool fillPerfTree = true;
  for ( string schip : schips ) {
    string dataset = datagroup + "_" + schip;
    cout << endl;
    cout << "*************  " << dataset << "  *****************" << endl;
    AdcChipAnalyzer chipper(dataset, 0, 16, datasetCalib, fillCalTree, vmin, vmax, nv, vresmax, fillPerfTree);
  }
}
