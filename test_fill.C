int test_fill(string aname ="") {
  map<string, string> tests;
  tests["03b"] = ".X filldb03b.C(1, 1, 3,1)";
  vector<string> allnames = {
    "03b"
  };
  vector<string> names;
  if ( aname == "" || aname == "all" ) names = allnames;
  else names.push_back(aname);
  int rstat = 0;
  int rtot = 0;
  string line = "=============================================================";
  cout << line << endl;
  for ( string name : names ) {
    string com = tests[name];
    if ( com.size() == 0 ) {
      cout << "Test not found: " << name << endl;
      rstat = 100;
    } else {
      cout << "Running test " << name << ": " << com << endl;
      rstat = gROOT->ProcessLine(com.c_str());
      cout << "Return code: " << rstat << endl;
    }
    rtot += abs(rstat);
    cout << endl;
    cout << line << endl;
  }
  cout << "Summed return status: " << rtot << endl;
  if ( rtot == 0 ) cout << "All tests pass." << endl;
  else cout << "One or more tests failed." << endl;
  return rtot;
}
