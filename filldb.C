void filldb() {
  vector<string> schips = {"00", "02", "03", "04", "05", "06", "07", "08",
                           "10", "14", "15", "16", "17", "18",
                           "21", "22", "24", "25", "26", "29",
                           "30", "31", "32", "33", "35" };
  bool filltree = false;
  unsigned int nv = 100;
  double vmin = -200.0;
  double vmax = 1800.0;
  double vrmsmax = 1.0;
  for ( string schip : schips ) {
    string dataset = "201701_" + schip;
    cout << endl;
    cout << "*************  " << dataset << "  *****************" << endl;
    AdcChipAnalyzer chipper(dataset, 0, 16, filltree, vmin, vmax, nv, vrmsmax);
  }
}
