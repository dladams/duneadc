void filldb03lt() {
  vector<string> schips = {"02"};
#if 0
  vector<string> sdays = {"06"};
  string datasetCalib = "";
#else
  //vector<string> sdays = {"06", "07"};
  vector<string> sdays = {"07"};
  string datasetCalib = "20170306ref";
#endif
  bool fillCalTree = true;
  unsigned int nv = 90;
  double vmin = -200.0;
  double vmax = 1600.0;
  double vresmax = 1.0;
  bool dropTails = false;
  bool fillPerfTree = true;
  for ( string schip : schips ) {
    for ( string sday : sdays ) {
      string dataset = "201703" + sday + "_" + schip;
      cout << endl;
      cout << "*************  " << dataset << "  *****************" << endl;
      AdcChipAnalyzer chipper(dataset, 0, 16, datasetCalib, fillCalTree, vmin, vmax, nv, vresmax, dropTails, fillPerfTree);
    }
  }
}
