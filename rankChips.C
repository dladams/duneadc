using MetricMap = map<unsigned int, double>;
using RankMap = multimap<double, unsigned int>;

void writePython(string name, const RankMap& chips);

TH1* rankChips(string dataset, int chip1 =1, int chip2 =80) {
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
    hists[0]->Fill(effprd);
    if ( isMarSurvey && chip < 41 ) hists[1]->Fill(effprd);
    if ( isMarSurvey && chip > 40 ) hists[2]->Fill(effprd);
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
  TCanvas* pcan = new TCanvas;
  pcan->SetRightMargin(0.03);
  hists[0]->Draw();
  TLegend* pleg = new TLegend(0.20, 0.70, 0.40, 0.85);
  pleg->SetBorderSize(0);
  pleg->SetFillStyle(0);
  for ( unsigned int ih=0; ih<hists.size(); ++ih ) {
    TH1* ph = hists[ih];
    string slab = slabs[ih];
    ph->Draw("same");
    pleg->AddEntry(ph, slab.c_str(), "l");
  }
  pleg->Draw();
  string fname = "chipQuality_" + dataset + ".png";
  pcan->Print(fname.c_str());
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

void rankChipsRef(string dataset ="DUNE17-cold", int chip1 =20, int chip2 =70,
                  string dataset2 ="201703a_mar25", int chip21 =1, int chip22 =80) {
  TH1* phr = rankChips(dataset2, chip21, chip22);
  if ( phr == nullptr ) return;
  TH1* pht = rankChips(dataset, chip1, chip2);
  if ( pht == nullptr ) return;
  phr->SetLineWidth(4);
  phr->SetLineColor(28);
  phr->SetLineStyle(2);
  double ymaxr = phr->GetMaximum();
  double ymaxt = pht->GetMaximum();
  if ( ymaxr > ymaxt ) pht->SetMaximum(1.03*ymaxt);
  pht->Draw();
  phr->Draw("same");
  pht->Draw("same");
  TLegend* pleg = new TLegend(0.20, 0.73, 0.50, 0.85);
  pleg->SetBorderSize(0);
  pleg->SetFillStyle(0);
  pleg->AddEntry(phr, dataset2.c_str(), "l");
  pleg->AddEntry(pht, dataset.c_str(), "l");
  pleg->Draw();
}
