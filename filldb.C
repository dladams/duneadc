void filldb() {
  vector<string> schips = {"00", "01", "02", "03", "04", "05", "06", "07", "08",
                           "10", "14", "15", "16", "17", "18",
                           "21", "22", "24", "25", "26", "29",
                           "30", "31", "32", "33", "35" };
  for ( string schip : schips ) {
    string dataset = "201701_" + schip;
    cout << endl;
    cout << "*************  " << dataset << "  *****************" << endl;
    makeplot(dataset, 0, 16, true);
  }
}
