void filldb02() {
  vector<string> schips = {"02", "04"};
  //vector<string> sdays = {"jan", "16", "17", "18", "19", "20", "21", "22", "23"};
  //vector<string> sdays = {"16"}; // For reference
  vector<string> sdays = {"janb"}; // For reference
  string datasetCalib = "20170216ref";
  bool fillCalTree = true;
  unsigned int nv = 90;
  double vmin = -200.0;
  double vmax = 1600.0;
  double vresmax = 1.0;
  bool fillPerfTree = true;
  for ( string schip : schips ) {
    for ( string sday : sdays ) {
      string dataset = "201702" + sday + "_" + schip;
      if ( sday == "jan" ) dataset = "201701c_" + schip;
      if ( sday == "janb" ) dataset = "201701b_" + schip;
      cout << endl;
      cout << "*************  " << dataset << "  *****************" << endl;
      AdcChipAnalyzer chipper(dataset, 0, 16, datasetCalib, fillCalTree, vmin, vmax, nv, vresmax, fillPerfTree);
    }
  }
}
