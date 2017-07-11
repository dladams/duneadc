using MetricMap = map<string, double>;
using RankMap = multimap<double, string>;

void writePython(string name, const RankMap& chips);

// Performanc is taken from perf_dsperf
TH1* rankChips(string dataset, string a_dslist ="", bool makeCan =true) {
  string myname = "rankChips: ";
  string dslist = a_dslist.size() ? a_dslist : dataset;
  RankMap rankedChipsAvg;
  RankMap rankedChipsPrd;
  MetricMap metricPrd;
  MetricMap metricAvg;
  MetricMap metricLow;
  RankMap goodChips;
  RankMap fairChips;
  RankMap poorChips;
  RankMap badChips;
  ostringstream sspymdst;
  ostringstream sspymsam;
  ostringstream sspymchp;
  ostringstream sspymavg;
  ostringstream sspymprd;
  ostringstream sspymlow;
  sspymdst << "datasetName = \"" << dataset << "\"";
  sspymdst << "datasetList = \"" << dslist << "\"";
  sspymavg << "effAvg = {";
  sspymprd << "effProduct = {";
  sspymlow << "effLow = {";
  int nhbin = 20;
  bool isMarSurvey = dataset == "201703a";
  vector<TH1*> hists;
  vector<string> slabs;
  string htitl = dataset + " ADC chip quality; Q; # chips";
  string hname = "heffprd_" + dataset;
  for ( string::size_type ipos=0; ipos<hname.size(); ++ipos ) {
    if ( hname[ipos] == '-' ) hname[ipos] = '_';
  }
  hists.push_back(new TH1F(hname.c_str(), htitl.c_str(), nhbin, 0, 1.0));
  slabs.push_back("All");
  if ( isMarSurvey ) {
    htitl = dataset + " ADC chip quality for Quik-Pak; Q; # chips";
    hists.push_back(new TH1F("heffprdQuik", htitl.c_str(), nhbin, 0, 1.0));
    hists.back()->SetLineColor(kRed+1);
    slabs.push_back("Quik-Pak");
    hists.back()->SetLineStyle(2);
    htitl = dataset + " ADC chip quality for MOSIS; Q; # chips";
    hists.push_back(new TH1F("heffprdMosis", htitl.c_str(), nhbin, 0, 1.0));
    hists.back()->SetLineColor(kGreen+2);
    hists.back()->SetLineStyle(3);
    slabs.push_back("MOSIS");
  }
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
  bool first = true;
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
  for ( string ssam : ssams ) {
    cout << myname << "Sample " << ssam << endl;
    //AdcChipMetric acm(dataset, chip);
    AdcChipMetric acm(dataset, ssam);
    int estat = acm.evaluate();
    if ( estat != 0 ) continue;
    double effavg = acm.chipMetric("EffAvg");
    double effprd = acm.chipMetric("EffProd");
    double efflow = acm.chipMetric("EffLow");
    double rankavg = 1.0 - effavg;
    double rankprd = 1.0 - effprd;
    std::pair<double, string> valavg(rankavg, ssam);
    std::pair<double, string> valprd(rankprd, ssam);
    rankedChipsAvg.insert(valavg);
    rankedChipsPrd.insert(valprd);
    metricPrd[ssam] = effprd;
    metricAvg[ssam] = effavg;
    metricLow[ssam] = efflow;
    if ( effprd > 0.95 ) goodChips.insert(valavg);
    else if ( effprd > 0.85 ) fairChips.insert(valavg);
    else if ( effprd > 0.7 ) poorChips.insert(valavg);
    else badChips.insert(valavg);
    string prefix = first ? "" : ",";
    prefix += "\n";
    string qsam = "\"" + ssam + "\"";
    sspymavg << prefix << setw(wsam) << qsam << ":" << effavg;
    sspymprd << prefix << setw(wsam) << qsam << ":" << effprd;
    sspymlow << prefix << setw(wsam) << qsam << ":" << efflow;
    hists[0]->Fill(effprd);
    //if ( isMarSurvey && chip < 41 ) hists[1]->Fill(effprd);
    //if ( isMarSurvey && chip > 40 ) hists[2]->Fill(effprd);
  }
  sspymavg << "\n}";
  sspymprd << "\n}";
  sspymlow << "\n}";
  cout.precision(3);
  int rank = 0;
  ostringstream sspyrank;
  sspyrank << "effrank = [";
  for ( auto& rc : rankedChipsPrd ) {
    string ssam = rc.second;
    double effprd = metricPrd[ssam];
    double effavg = metricAvg[ssam];
    double efflow = metricLow[ssam];
    ++rank;
    if ( rank > 1 ) sspyrank << ", ";
    if ( rank%10 == 0 ) sspyrank << "\n";
    sspyrank << ssam;
    cout << setw(4) << rank << ": " << setw(4) << ssam
         << ": " << fixed << effprd
         << ", " << fixed << effavg
         << ", " << fixed << efflow << endl;
  }
  sspyrank << "]";
  writePython("good", goodChips);
  writePython("fair", fairChips);
  writePython("poor", poorChips);
  writePython("bad",  badChips);
  string ofname = "rank_" + dslist + ".py";
  for ( char& ch : ofname ) if ( ch == '-' ) ch = '_';
  ofstream outp(ofname.c_str());
  outp << sspymdst.str() << endl;
  outp << sspymavg.str() << endl;
  outp << sspymprd.str() << endl;
  outp << sspymlow.str() << endl;
  outp << sspyrank.str() << endl;
  cout << "Python output file: " << ofname << endl;
  TCanvas* pcan = nullptr;
  if ( makeCan ) {
    pcan = new TCanvas;
    pcan->SetRightMargin(0.03);
  }
  hists[0]->Draw();
  //TLegend* pleg = new TLegend(0.20, 0.70, 0.40, 0.85);
  //pleg->SetBorderSize(0);
  //pleg->SetFillStyle(0);
  //for ( unsigned int ih=0; ih<hists.size(); ++ih ) {
    //TH1* ph = hists[ih];
    //string slab = slabs[ih];
    //ph->Draw("same");
    //pleg->AddEntry(ph, slab.c_str(), "l");
  //}
  //pleg->Draw();
  string fname = "chipQuality_" + dataset + ".png";
  if ( pcan != nullptr ) pcan->Print(fname.c_str());
  return hists[0];
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
