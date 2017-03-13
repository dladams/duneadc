AdcDatasetAnalyzer* pada;

AdcDatasetAnalyzer* datasetAnalyze(string dsname, Index achip, Index ibin1 =1, Index ibin2 =4095) {
  vector<string> dsts;
  if ( dsname == "201702" ) {
    dsts.push_back("201701c");
    dsts.push_back("20170216");
    dsts.push_back("20170217");
    dsts.push_back("20170218");
    dsts.push_back("20170219");
    dsts.push_back("20170220");
    dsts.push_back("20170221");
    dsts.push_back("20170222");
    dsts.push_back("20170223");
  } else if ( dsname == "201703" ) {
    dsts.push_back("20170306");
    dsts.push_back("20170307");
  } else {
    cout << "Name not found: " << dsname << endl;
    return nullptr;
  }
  pada = new AdcDatasetAnalyzer(dsts, achip, ibin1, ibin2);
  return pada;
}
