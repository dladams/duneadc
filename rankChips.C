using SampleMetricMap = map<string, double>;
using ChipSample = std::pair<Index, string>;
using ValueChipSample = std::pair<double, ChipSample>;
using ChipMetricMap = map<Index, ValueChipSample>;
using RankMap = multimap<double, ChipSample>;

void writePython(string name, const RankMap& chips);

// Performanc is taken from perf_dsperf
TH1* rankChips(string dataset, string a_dslist ="", bool makeCan =true) {
  string myname = "rankChips: ";
  string dslist = a_dslist.size() ? a_dslist : dataset;
  RankMap rankedChipsPrd;
  SampleMetricMap metricPrd;
  SampleMetricMap metricAvg;
  SampleMetricMap metricLow;
  ChipMetricMap chipMetricPrd;
  RankMap goodChips;
  RankMap fairChips;
  RankMap poorChips;
  RankMap badChips;
  ostringstream sspymdst;
  ostringstream sspymdsl;
  ostringstream sspymsam;
  ostringstream sspymchp;
  ostringstream sspymavg;
  ostringstream sspymprd;
  ostringstream sspymlow;
  sspymdst << "datasetName = \"" << dataset << "\"";
  sspymdsl << "datasetList = \"" << dslist << "\"";
  sspymavg << "effAvg = {";
  sspymprd << "effProduct = {";
  sspymlow << "effLow = {";
  int nhbin = 20;
  vector<TH1*> hists;
  vector<string> slabs;
  string htitl = dataset + " ADC chip quality; Q_{max}; # chips";
  string hname = "heffprd_" + dataset;
  for ( string::size_type ipos=0; ipos<hname.size(); ++ipos ) {
    if ( hname[ipos] == '-' ) hname[ipos] = '_';
  }
  hists.push_back(new TH1F(hname.c_str(), htitl.c_str(), nhbin, 0, 1.0));
  slabs.push_back("All");
  for ( TH1* ph : hists ) {
    ph->SetStats(0);
    ph->SetLineWidth(2);
  }
  if ( hists.size() > 1 ) hists[0]->SetLineWidth(3);
  cout << endl;
  cout << myname << dataset << endl;
  string dsfname = dslist + ".txt";
  ifstream dsfile(dsfname.c_str());
  if ( ! dsfile ) {
    cout << myname << "Dastaset file not found: " << dsfname << endl;
    return nullptr;
  }
  vector<string> ssams;
  Index wsam = 0;
  while ( dsfile ) {
    string ssam;
    dsfile >> ssam;
    if ( dsfile.eof() ) break;
    ssams.push_back(ssam);
    Index lsam = ssam.size();
    if ( lsam > wsam ) wsam = lsam;
  }
  wsam += 4;
  bool first = true;
  // Loop over samples.
  for ( string ssam : ssams ) {
    //AdcChipMetric acm(dataset, chip);
    AdcChipMetric acm(dataset, ssam);
    int estat = acm.evaluate();
    Index chip = acm.chip();    // Must call this after evaluate.
    cout << myname << "Sample " << ssam << " (chip " << chip << ")" << endl;
    if ( estat != 0 ) continue;
    double effavg = acm.chipMetric("EffAvg");
    double effprd = acm.chipMetric("EffProd");
    double efflow = acm.chipMetric("EffLow");
    double rankavg = 1.0 - effavg;
    double rankprd = 1.0 - effprd;
    ChipSample chipsam(chip, ssam);
    ValueChipSample valprd(rankprd, chipsam);
    metricPrd[ssam] = effprd;
    metricAvg[ssam] = effavg;
    metricLow[ssam] = efflow;
    auto iprd = chipMetricPrd.find(chip);
    if ( iprd == chipMetricPrd.end() ||
         effprd > iprd->second.first ) {
      chipMetricPrd[chip] = valprd;
    }
    string prefix = first ? "" : ",";
    prefix += "\n";
    string qsam = "\"" + ssam + "\"";
    sspymavg << prefix << setw(wsam) << qsam << ":" << effavg;
    sspymprd << prefix << setw(wsam) << qsam << ":" << effprd;
    sspymlow << prefix << setw(wsam) << qsam << ":" << efflow;
    first = false;
  }
  // Loop over chips.
  int count = 0;
  for ( auto iprd : chipMetricPrd ) {
    Index chip = iprd.first;
    ValueChipSample valprd = iprd.second;
    double rankprd = valprd.first;
    double effprd = 1.0 - rankprd;
    ChipSample chipsam = valprd.second;
    ValueChipSample vcs(effprd, chipsam);
    rankedChipsPrd.insert(vcs);
    if ( effprd > 0.95 ) goodChips.insert(vcs);
    else if ( effprd > 0.85 ) fairChips.insert(vcs);
    else if ( effprd > 0.7 ) poorChips.insert(vcs);
    else badChips.insert(vcs);
    hists[0]->Fill(effprd);
  }
  sspymavg << "\n}";
  sspymprd << "\n}";
  sspymlow << "\n}";
  cout.precision(3);
  int rank = 0;
  ostringstream sspyrankSample;
  ostringstream sspyrankChip;
  sspyrankSample << "rankSample = [";
  sspyrankChip   << "rankChip = [";
  for ( auto& rc : rankedChipsPrd ) {
    Index chip = rc.second.first;
    string ssam = rc.second.second;
    double effprd = metricPrd[ssam];
    double effavg = metricAvg[ssam];
    double efflow = metricLow[ssam];
    ++rank;
    if ( rank > 1 ) {
      sspyrankSample << ", ";
      sspyrankChip << ", ";
    }
    sspyrankSample << "\n  ";
    sspyrankChip << "\n  ";
    string qsam = "\"" + ssam + "\"";
    sspyrankSample << qsam;
    sspyrankChip << chip;
    cout << setw(4) << rank << ": " << setw(4) << ssam
         << ": " << fixed << effprd
         << ", " << fixed << effavg
         << ", " << fixed << efflow << endl;
  }
  sspyrankSample << "]";
  sspyrankChip << "]";
  writePython("good", goodChips);
  writePython("fair", fairChips);
  writePython("poor", poorChips);
  writePython("bad",  badChips);
  string ofname = "rank_" + dslist + ".py";
  for ( char& ch : ofname ) if ( ch == '-' ) ch = '_';
  ofstream outp(ofname.c_str());
  outp << sspymdst.str() << endl;
  outp << sspymdsl.str() << endl;
  outp << sspymavg.str() << endl;
  outp << sspymprd.str() << endl;
  outp << sspymlow.str() << endl;
  outp << sspyrankSample.str() << endl;
  outp << sspyrankChip.str() << endl;
  cout << "Python output file: " << ofname << endl;
  TCanvas* pcan = nullptr;
  if ( makeCan ) {
    pcan = new TCanvas;
    pcan->SetRightMargin(0.03);
  }
  Index nchip = rankedChipsPrd.size();
  ostringstream sshtitl;
  sshtitl << dataset << " ADC chip quality (" << nchip << " chips)";
  htitl = sshtitl.str();
  hists[0]->SetTitle(htitl.c_str());
  hists[0]->Draw();
  string fname = "chipQuality_" + dataset + ".png";
  if ( pcan != nullptr ) pcan->Print(fname.c_str());
  cout << "Sample count: " << metricPrd.size() << "/" << ssams.size() << endl;
  cout << "  Chip count: " << nchip << endl;
  return hists[0];
}

void writePython(string name, const RankMap& chips) {
  unsigned int count = 0;
  cout << "\"" << name << "\" : [";
  for ( auto& rc : chips ) {
    if ( count++ ) cout << ", ";
    cout << rc.second.second;
  }
  cout << "]" << endl;
}

void rankChipsRef(string dataset ="DUNE17-cold", string dslist ="",
                  string dataset2 ="201703a_mar25", string dslist2 ="") {
  pcan = new TCanvas;
  pcan->SetRightMargin(0.03);
  bool makeCan = false;
  TH1* phr = rankChips(dataset2, dslist2, makeCan);
  if ( phr == nullptr ) return;
  TH1* pht = rankChips(dataset, dslist, makeCan);
  if ( pht == nullptr ) return;
  int nchip = pht->GetEntries();
  cout << "Test dataset chip count: " << nchip << endl;
  phr->SetLineWidth(4);
  phr->SetLineColor(28);
  phr->SetLineStyle(2);
  double ymaxr = phr->GetMaximum();
  double ymaxt = pht->GetMaximum();
  if ( ymaxr > ymaxt ) pht->SetMaximum(1.03*ymaxr);
  pht->Draw();
  phr->Draw("same");
  pht->Draw("same");
  TLegend* pleg = new TLegend(0.20, 0.73, 0.60, 0.85);
  pleg->SetBorderSize(0);
  pleg->SetFillStyle(0);
  ostringstream sslab;
  sslab << dataset << " (" << nchip << " chips)";
  string slab = sslab.str();
  pleg->AddEntry(phr, dataset2.c_str(), "l");
  pleg->AddEntry(pht, slab.c_str(), "l");
  pleg->Draw();
  string fname = "rank_" + dataset + "_ref" + dataset2 + ".png";
  pcan->Print(fname.c_str());
}
