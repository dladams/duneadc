void filldb_ltc() {
  string datasetCalib;
  bool fillCalTree = true;
  unsigned int nv = 90;
  double vmin = -200.0;
  double vmax = 1600.0;
  double vresmax = 1.0;
  bool fillPerfTree = true;
  string dataset = "201612_ltc2314";
  cout << endl;
  cout << "*************  " << dataset << "  *****************" << endl;
  AdcChipAnalyzer chipper(dataset, 0, 4, datasetCalib, fillCalTree, vmin, vmax, nv, vresmax, fillPerfTree);
}
