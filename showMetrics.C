int showMetrics(string samName) {
  const string myname = "showMetrics: ";
  string::size_type ipos = samName.find("_chip");
  if ( ipos == string::npos ) return 1;
  string dstName = samName.substr(0, ipos);
  string fname = samName + "/perf_" + samName + ".root";
  AdcChipMetric acm(dstName, samName, fname);
  int nbad = acm.evaluate();
  if ( nbad >= 0 ) acm.print();
  else {
    cout << myname << "Metric evaluation failed." << endl;
    return nbad;
  }
  return 0;
}
  
