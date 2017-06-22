int test_all() {
  vector<string> names = {
    "AdcChannelId",
    "AdcSampleFinder",
    "AdcFembTreeSampleReader",
    "AdcCalibrationTree",
    "AdcBinRecord",
    "AdcSampleAnalyzer",
    "AdcChipAnalyzer",
    "AdcLinearChannelCalibration",
    "AdcPedestalChannelCalibration"
  };
  string allline = "=============================================================";
  int rtot = 0;
  for ( string name : names ) {
    cout << endl;
    cout << allline << endl;
    cout << name << endl;
    cout << allline << endl;
    string com = ".X test_" + name + ".cxx+";
    int rstat = gROOT->ProcessLine(com.c_str());
    cout << "Return code: " << rstat << endl;
    rtot += abs(rstat);
  }
  cout << endl;
  cout << allline << endl;
  cout << "Summed return status: " << rtot << endl;
  if ( rtot == 0 ) cout << "All tests pass." << endl;
  else cout << "One or more tests failed." << endl;
  return rtot;
}
