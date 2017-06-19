int test_all() {
  vector<string> names = {
    "AdcCalibrationTree",
    "AdcBinRecord" };
  string line = "=============================================================";
  for ( string name : names ) {
    cout << endl;
    cout << line << endl;
    cout << name << endl;
    cout << line << endl;
    string com = ".X test_" + name + ".cxx+";
    int rstat = gROOT->ProcessLine(com.c_str());
    cout << "Return code: " << rstat << endl;
  }
  return 0;
}
