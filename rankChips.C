using MetricMap = map<unsigned int, double>;
using RankMap = multimap<double, unsigned int>;

void writePython(string name, const RankMap& chips);

void rankChips(string dataset, int chip1 =1, int chip2 =80) {
  string myname = "rankChips: ";
  RankMap rankedChipsAvg;
  RankMap rankedChipsPrd;
  MetricMap metricPrd;
  MetricMap metricAvg;
  MetricMap metricLow;
  RankMap goodChips;
  RankMap fairChips;
  RankMap poorChips;
  RankMap badChips;
  ostringstream sspymavg;
  ostringstream sspymprd;
  ostringstream sspymlow;
  sspymavg << "effavg = {";
  sspymprd << "effprd = {";
  sspymlow << "efflow = {";
  for ( unsigned int  chip=chip1; chip<=chip2; ++chip ) {
    cout << myname << "Chip " << chip << endl;
    AdcChipMetric acm(dataset, chip);
    acm.evaluate();
    double effavg = acm.chipMetric("EffAvg");
    double effprd = acm.chipMetric("EffProd");
    double efflow = acm.chipMetric("EffLow");
    double rankavg = 1.0 - effavg;
    double rankprd = 1.0 - effprd;
    std::pair<double, unsigned int> valavg(rankavg, chip);
    std::pair<double, unsigned int> valprd(rankprd, chip);
    rankedChipsAvg.insert(valavg);
    rankedChipsPrd.insert(valprd);
    metricPrd[chip] = effprd;
    metricAvg[chip] = effavg;
    metricLow[chip] = efflow;
    if ( effprd > 0.95 ) goodChips.insert(valavg);
    else if ( effprd > 0.85 ) fairChips.insert(valavg);
    else if ( effprd > 0.7 ) poorChips.insert(valavg);
    else badChips.insert(valavg);
    string prefix = chip > chip1 ? ", " : "";
    if ( chip%10 == 0 ) prefix += "\n";
    sspymavg << prefix << chip << ":" << effavg;
    sspymprd << prefix << chip << ":" << effprd;
    sspymlow << prefix << chip << ":" << efflow;
  }
  sspymavg << "}";
  sspymprd << "}";
  sspymlow << "}";
  cout.precision(3);
  int rank = 0;
  ostringstream sspyrank;
  sspyrank << "effrank = [";
  for ( auto& rc : rankedChipsPrd ) {
    unsigned int chip = rc.second;
    double effprd = metricPrd[chip];
    double effavg = metricAvg[chip];
    double efflow = metricLow[chip];
    ++rank;
    if ( rank > 1 ) sspyrank << ", ";
    if ( rank%10 == 0 ) sspyrank << "\n";
    sspyrank << chip;
    cout << setw(4) << rank << ": " << setw(4) << chip
         << ": " << fixed << effprd
         << ", " << fixed << effavg
         << ", " << fixed << efflow << endl;
  }
  sspyrank << "]";
  writePython("good", goodChips);
  writePython("fair", fairChips);
  writePython("poor", poorChips);
  writePython("bad",  badChips);
  cout << sspymavg.str() << endl;
  cout << sspymprd.str() << endl;
  cout << sspymlow.str() << endl;
  cout << sspyrank.str() << endl;
}

void writePython(string name, const RankMap& chips) {
  unsigned int count = 0;
  cout << "\"" << name << "\" : [";
  for ( auto& rc : chips ) {
    if ( count++ ) cout << ", ";
    cout << rc.second;
  }
  cout << "]" << endl;
}
