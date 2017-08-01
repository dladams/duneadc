using SampleMetricMap = map<string, double>;
using SampleIndexMap = map<string, Index>;
using StringVector = vector<string>;
using StringStringMap = map<string,string>;
using ChipSample = std::pair<Index, string>;
using ValueChipSample = std::pair<double, ChipSample>;
using ChipMetricMap = map<Index, ValueChipSample>;
using RankMap = multimap<double, ChipSample>;
using ChanValMap = map<string, std::vector<double>>;
using DoubleVector = vector<double>;
using DoubleVectorVector = vector<DoubleVector>;

void writePython(string name, const RankMap& chips);

// Performance is taken from perf_dsperf
TH1* rankChips(string datasetString="PDTS:CETS", string a_dslist ="DUNE17all-cold-rem", SampleMetricMap* pmets =nullptr) {
  string myname = "rankChips: ";
  string::size_type ipos = 0;
  vector<string> datasets;
  while ( ipos != string::npos ) {
    string::size_type jpos = datasetString.find(":", ipos);
    datasets.push_back(datasetString.substr(ipos, jpos-ipos));
    ipos = jpos;
    if ( ipos != string::npos ) ++ipos;
  }
  cout << myname << "Datasets assumed in performance evaluation:" << endl;
  for ( string dst : datasets ) {
    cout << myname << "  " << dst << endl;
  }
  string dslist = a_dslist.size() ? a_dslist : datasets[0];
  SampleIndexMap sampleChip;
  RankMap rankedChipsPrd;
  SampleMetricMap metricPrd;
  SampleMetricMap metricAvg;
  SampleMetricMap metricLow;
  SampleIndexMap metricLo2;
  SampleMetricMap metricN80;
  ChipMetricMap chipMetricPrd;
  RankMap goodChips;
  RankMap fairChips;
  RankMap poorChips;
  RankMap badChips;
  ChanValMap chaneff;
  StringStringMap sampleDataset;
  ostringstream sspymdst;
  ostringstream sspymdsl;
  ostringstream sspymsam;
  ostringstream sspymchp;
  ostringstream sspymavg;
  ostringstream sspymprd;
  ostringstream sspymlow;
  ostringstream sspymlo2;
  ostringstream sspymn80;
  sspymdst << "datasetName = \"" << datasetString << "\"";
  sspymdsl << "datasetList = \"" << dslist << "\"";
  sspymavg << "effAvg = {";
  sspymprd << "effProduct = {";
  sspymlow << "effLow = {";
  sspymlo2 << "effLow2 = {";
  sspymn80 << "n80 = {";
  int nhbin = 20;
  vector<TH1*> hists;
  vector<string> slabs;
  Index ndst = datasets.size();
  Index nhst = 1;
  vector<string> histDatasetNames;
  if ( ndst > 1 ) nhst = ndst + 1;
  // Create Qmax histos.
  for ( Index ihst=0; ihst<nhst; ++ihst ) {
    string dsname = ihst==0 ? dslist : datasets[ihst-1];
    histDatasetNames.push_back(dsname);
    slabs.push_back(dsname);
    string hname = "heffprd_" + dsname;
    for ( string::size_type ipos=0; ipos<hname.size(); ++ipos ) {
      if ( hname[ipos] == '-' ) hname[ipos] = '_';
    }
    string htitl = dsname + " ADC chip quality; Q_{max}; # chips";
    hists.push_back(new TH1F(hname.c_str(), htitl.c_str(), nhbin, 0, 1.0));
  }
  if ( nhst > 1 ) slabs[0] = "All";
  for ( TH1* ph : hists ) {
    ph->SetStats(0);
    ph->SetLineWidth(2);
  }
  cout << endl;
  cout << myname << dslist << endl;
  string dsfname = dslist + ".txt";
  ifstream dsfile(dsfname.c_str());
  if ( ! dsfile ) {
    cout << myname << "Dataset file not found: " << dsfname << endl;
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
  // Read the samples for each dataset.
  std::vector<vector<string>> datasetSamples(ndst);
  for ( Index idst=0; idst<ndst; ++idst ) {
    string dataset = datasets[idst];
    string dsfname = dataset +".txt";
    ifstream dsf(dsfname.c_str());
    if ( ! dsf ) {
      cout << myname << "Dataset description file not found: " << dsfname << endl;
      return nullptr;
    }
    while ( dsf ) {
      string ssam;
      dsf >> ssam;
      if ( dsf.eof() ) break;
      datasetSamples[idst].push_back(ssam);
    }
    cout << "Dataset " << dataset << " sample count: " << datasetSamples[idst].size() << endl;
  }
  bool first = true;
  // Loop over samples.
  map<string,Index> datasetIndex;    // Dataset index for each sample. ndst if not found.
  Index ncha = 0;
  for ( string ssam : ssams ) {
    //AdcChipMetric acm(dataset, chip);
    std::unique_ptr<AdcChipMetric> pacm;
    int estat = 99;
    Index idst = 0;
    string dataset;
    sampleDataset[ssam] = "DatasetNotFound";
    for ( ; idst<datasets.size(); ++idst ) {
      dataset = datasets[idst];
      const vector<string>& sams = datasetSamples[idst];
      if ( find(sams.begin(), sams.end(), ssam)  != sams.end() ) {
        sampleDataset[ssam] = dataset;
        pacm.reset(new AdcChipMetric(dataset, ssam));
        estat = pacm->evaluate();
        if ( estat == 0 ) break;
      }
    }
    if ( idst == datasets.size() || estat != 0 ) {
      cout << myname << "Unable to find performance for " << ssam << endl;
      continue;
    }
    datasetIndex[ssam] = idst;
    AdcChipMetric& acm = *pacm;
    if ( acm.nChannel() == 0 ) {
      cout << myname << "Channel count is zero for " << ssam << endl;
      continue;
    }
    if ( ncha == 0 ) ncha = acm.nChannel();
    if ( acm.nChannel() != ncha ) {
      cout << myname << "Inconsistent channel count for " << ssam << endl;
      continue;
    }
    Index chip = acm.chip();    // Must call this after evaluate.
    sampleChip[ssam] = chip;
    cout << myname << "Sample " << ssam << " (chip " << chip << ")" << endl;
    if ( estat != 0 ) continue;
    double effavg = acm.chipMetric("EffAvg");
    double effprd = acm.chipMetric("EffProd");
    double efflow = acm.chipMetric("EffLow");
    double efflo2 = acm.chipMetric("EffLow2");
    int n80 = acm.chipMetric("N80");
    double rankavg = 1.0 - effavg;
    double rankprd = 1.0 - effprd;
    ChipSample chipsam(chip, ssam);
    ValueChipSample valprd(rankprd, chipsam);
    metricPrd[ssam] = effprd;
    metricAvg[ssam] = effavg;
    metricLow[ssam] = efflow;
    metricLo2[ssam] = efflo2;
    metricN80[ssam] = n80;
    for ( Index icha=0; icha<ncha; ++icha ) {
      chaneff[ssam].push_back(acm.channelEfficiency(icha));
    }
    auto iprd = chipMetricPrd.find(chip);
    bool insert = iprd == chipMetricPrd.end();
    if ( ! insert ) {
      string oldsam = iprd->second.second.second;
      double oldeff = metricPrd[oldsam];
      insert = effprd > oldeff;
    }
    if ( insert ) chipMetricPrd[chip] = valprd;
    string prefix = first ? "" : ",";
    prefix += "\n";
    string qsam = "\"" + ssam + "\"";
    sspymavg << prefix << setw(wsam) << qsam << ":" << effavg;
    sspymprd << prefix << setw(wsam) << qsam << ":" << effprd;
    sspymlow << prefix << setw(wsam) << qsam << ":" << efflow;
    sspymlo2 << prefix << setw(wsam) << qsam << ":" << efflo2;
    sspymn80 << prefix << setw(wsam) << qsam << ":" << n80;
    first = false;
  }
  // Loop over chips.
  int count = 0;
  vector<Index> n80count(17,0);
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
    if ( nhst > 1 ) {
      Index idst = datasetIndex[chipsam.second];
      hists[idst+1]->Fill(effprd);
    }
    Index n80 = metricN80[chipsam.second];
    ++n80count[n80];
  }
  sspymavg << "\n}";
  sspymprd << "\n}";
  sspymlow << "\n}";
  sspymlo2 << "\n}";
  sspymn80 << "\n}";
  cout.precision(3);
  int rank = 0;
  ostringstream sspyrankSample;
  ostringstream sspyrankChip;
  sspyrankSample << "rankedSampleByChip = {";
  sspyrankChip   << "rankChip = [";
  //for ( auto& rc : rankedChipsPrd ) {
  string outsumName = "rank_" + dslist + ".txt";
  string outsum2Name = "rank_" + dslist + "_long.txt";
  ofstream outsum(outsumName.c_str());
  ofstream outsum2(outsum2Name.c_str());
  ostringstream outl;
  int wds = 7;
  int wval = 8;
  for ( string dataset : datasets ) if ( dataset.size() > wds ) wds = dataset.size();
  wds += 2;
  outl << setw(5) << "Rank" << setw(6) << "Chip" << setw(wval) << "Q" << setw(4) << "N80";
  if ( ndst > 1 ) outl << setw(wds) << "Dataset";
  outsum << outl.str() << endl;
  for ( Index icha=0; icha<ncha; ++icha ) {
    outl << setw(wval-2) << "eff";
    if ( icha < 10 ) outl << 0;
    outl << icha;
  }
  outsum2 << outl.str() << endl;
  for ( auto irc=rankedChipsPrd.rbegin(); irc!=rankedChipsPrd.rend(); ++irc ) {
    auto rc = *irc;
    Index chip = rc.second.first;
    string ssam = rc.second.second;
    double effprd = metricPrd[ssam];
    double effavg = metricAvg[ssam];
    double efflow = metricLow[ssam];
    Index n80 = metricN80[ssam];
    ++rank;
    if ( rank > 1 ) {
      sspyrankSample << ", ";
      sspyrankChip << ", ";
    }
    sspyrankSample << "\n  ";
    sspyrankChip << "\n  ";
    string qsam = "\"" + ssam + "\"";
    sspyrankChip << chip;
    sspyrankSample << chip << ":" << qsam;
    cout << setw(4) << rank << ": " << setw(4) << ssam
         << ": " << fixed << effprd
         << ", " << fixed << effavg
         << ", " << fixed << efflow << endl;
    Index idst = datasetIndex[ssam];
    outl.str("");
    outl << setw(5) << rank << setw(6) << chip
         << setw(wval) << fixed << setprecision(3) << effprd
         << setw(4) << n80;
    if ( ndst > 1 ) outl << setw(wds) << datasets[idst];
    outsum << outl.str() << endl;
    for ( Index icha=0; icha<ncha; ++icha ) {
      outl << setw(wval) << fixed << setprecision(3) << chaneff[ssam][icha];
    }
    outsum2 << outl.str() << endl;
  }
  cout << "Summary output file: " << outsumName << endl;
  cout << "Long summary output file: " << outsum2Name << endl;
  sspyrankSample << "}";
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
  outp << sspymlo2.str() << endl;
  outp << sspymn80.str() << endl;
  outp << sspyrankSample.str() << endl;
  outp << sspyrankChip.str() << endl;
  cout << "Python output file: " << ofname << endl;
  // Draw quality histogram.
  Index nchip = rankedChipsPrd.size();
  ostringstream sshtitl;
  sshtitl << dslist << " ADC chip quality (" << nchip << " chips)";
  dyleg = 0.05*nhst;
  double xleg1 = 0.40;
  double xleg2 = xleg1 + 0.30;
  double yleg2 = 0.85;
  double yleg1 = yleg2 - dyleg;
  htitl = sshtitl.str();
  TH1* ph0 = hists[0];
  ph0->SetLineWidth(3);
  double ymax = ph0->GetMaximum();
  double ybinmax = 0.0;
  for ( ibin=4; ibin<=ph0->GetNbinsX(); ++ibin ) {
    double ybin = ph0->GetBinContent(ibin);
    if ( ybin > ybinmax ) ybinmax = ybin;
  }
  if ( ybinmax < 0.7*ymax ) {
    ymax = int(1.3*ybinmax+0.5);
    ph0->SetMinimum(0.0);
    ph0->SetMaximum(ymax);
  }
  string fnameQuality = "chipQuality_" + dslist + ".png";
  // Draw quality overlaying chip lists.
  int colPDTS1 = kMagenta+2;
  int colPDTS2 = 46;          // Red
  int colCETS = kGreen + 3;
  int colCETSPDTS = 28;       // Brown
  if ( nhst > 1 ) {
    pcan = new TCanvas;
    pcan->SetRightMargin(0.03);
    hists[0]->Draw();
    hists[0]->SetMinimum(0.0);
    TLegend* pleg = new TLegend(xleg1, yleg1, xleg2, yleg2);
    pleg->SetBorderSize(0);
    pleg->SetFillStyle(0);
    for ( Index ihst=0; ihst<nhst; ++ihst ) {
      TH1* ph = hists[ihst];
      Index nbin = ph->GetNbinsX();
      Index bin1 = nbin/2 + 1;
      Index nhalf = ph->Integral(bin1, nbin+1);
      Index ntot = ph->GetEntries();
      ostringstream sslab;
      sslab << slabs[ihst] << " (" << nhalf << "/" << ntot << " chips)";
      string slab = sslab.str();
      if ( ihst == 1 ) ph->SetLineColor(colPDTS1);
      if ( ihst == 2 ) ph->SetLineColor(colPDTS2);
      if ( ihst == 3 ) ph->SetLineColor(colCETS);
      if ( ihst == 1 ) ph->SetLineStyle(1);
      if ( ihst == 2 ) ph->SetLineStyle(3);
      if ( ihst == 3 ) ph->SetLineStyle(2);
      if ( ihst == 1 ) ph->SetLineWidth(1);
      if ( ihst == 2 ) ph->SetLineWidth(3);
      if ( ihst == 3 ) ph->SetLineWidth(2);
      ph->Draw("same");
      pleg->AddEntry(ph, slab.c_str(), "l");
    }
    pleg->Draw();
    if ( pcan != nullptr ) pcan->Print(fnameQuality.c_str());
    // Change name for combined plot if we also have multi-list plot.
    fnameQuality = "chipQualityCombined_" + dslist + ".png";
  }
  // Draw combined quality overlaying chip lists.
  pcan = new TCanvas;
  pcan->SetRightMargin(0.03);
  TH1* ph = hists[0];
  ostringstream sstitl;
  sstitl << ph->GetTitle() << " (" << ph->GetEntries() << " chips)";
  htitl = sstitl.str();
  ph->SetTitle(htitl.c_str());
  ph->Draw();
  if ( pcan != nullptr ) pcan->Print(fnameQuality.c_str());
  // Build metric vectors.
  vector<double> valAvg;
  for ( SampleMetricMap::value_type ient : metricAvg ) valAvg.push_back(pow(ient.second,16));
  vector<double> valPrd;
  for ( SampleMetricMap::value_type ient : metricPrd ) valPrd.push_back(ient.second);
  vector<double> valLow;
  for ( SampleMetricMap::value_type ient : metricLow ) valLow.push_back(ient.second);
  // Plot Eavg vs Q.
  TGraph* pgr = new TGraph(valAvg.size(), &valPrd.front(), &valAvg.front());
  string hnam = "haxmcor_" + dslist;
  sshtitl.str("");
  sshtitl << dslist << " <#varepsilon>-Q correlation (" << valAvg.size()
          << " samples); Q = #Pi #varepsilon_{chan}; <#varepsilon_{chan}>^{16}";
  htitl = sshtitl.str();
  TH2* phax = new TH2F(hnam.c_str(), htitl.c_str(), 10, 0, 1, 10, 0, 1);
  phax->SetStats(0);
  pcan = new TCanvas;
  pcan->SetRightMargin(0.03);
  pgr->SetMarkerStyle(2);
  phax->Draw();
  pgr->Draw("p");
  fname = "chipQualityAvgCorrelation_" + dslist + ".png";
  if ( pcan != nullptr ) pcan->Print(fname.c_str());
  // Plot Emin vs Q.
  TGraph* pgr2 = new TGraph(valPrd.size(), &valPrd.front(), &valLow.front());
  hnam = "haxmcor_" + dslist;
  sshtitl.str("");
  sshtitl << dslist << " #varepsilon_{min}-Q correlation (" << valLow.size()
          << " samples); Q = #Pi #varepsilon_{chan}; min(#varepsilon_{chan})";
  htitl = sshtitl.str();
  TH2* phax2 = new TH2F(hnam.c_str(), htitl.c_str(), 10, 0, 1, 10, 0, 1);
  phax2->SetStats(0);
  pcan = new TCanvas;
  pcan->SetRightMargin(0.03);
  pgr2->SetMarkerStyle(2);
  phax2->Draw();
  pgr2->Draw("p");
  fname = "chipQualityLowCorrelation_" + dslist + ".png";
  if ( pcan != nullptr ) pcan->Print(fname.c_str());
  // Plot q correlations: Q vs Qchip_max
  const int i11 = 0;
  const int i22 = 1;
  const int iCC = 2;
  const int i2C = 3;
  const int i21 = 4;
  const int iC1 = 5;
  const int nxx = 6;
  StringVector slab{"PDTS1 PDTS1", "PDTS2 PDTS2", "CETS CETS", "PDTS2 CETS", "PDTS2 PDTS1", "CETS PDTS1"};
  vector<int> qmrk = {2, 2, 2,  4, 26, 32};
  vector<int> qcol = {colPDTS1, colPDTS2, colCETS, colCETSPDTS, colPDTS2, colCETS};
  DoubleVectorVector qcx(nxx);
  DoubleVectorVector qcy(nxx);
  for ( string ssam : ssams ) {
    Index chip = sampleChip[ssam];
    auto iprd = chipMetricPrd.find(chip);
    string dst = sampleDataset[ssam];
    double q = metricPrd[ssam];
    if ( iprd != chipMetricPrd.end() ) {
      Index chip0 = iprd->second.second.first;
      string sam0 = iprd->second.second.second;
      string dst0 = sampleDataset[sam0];
      double q0 = metricPrd[sam0];
      if ( sam0 != ssam ) {
        double q1 = metricPrd[sam0];
        double q2 = q;
        string dst1 = dst0;
        string dst2 = dst;
        bool flip = false;
        if ( dst1 == dst2 ) {
          flip = q2 > q1;
        } else {
          flip |= dst2 == "PDTS2";
          flip |= dst1 == "PDTS1";
        }
        if ( flip ) {
          q2 = q1;
          q1 = q;
          dst1 = dst;
          dst2 = dst0;
        }
        unsigned int ixx = nxx;
        string lab = dst1 + " " + dst2;
        for ( ixx=0; ixx<nxx; ++ixx ) {
          if ( slab[ixx] == lab ) break;
        }
        if ( ixx == nxx ) {
          cout << myname << "Unable to find dataset pair index." << endl;
          continue;
        }
        qcx[ixx].push_back(q1);
        qcy[ixx].push_back(q2);
        if ( 1 ) cout << setw(12) << slab[ixx] << "  qmax  q: " << q1 << "    "
                      << q2 << "   " << sam0  << ", " << ssam << endl;
      }
    } else {
      cout << myname << "ERROR: Sample " << ssam << " chip " << chip << " is not in chip metric map!" << endl;
    }
  }
  vector<TGraph*> qgr(nxx, nullptr);
  dyleg = 0.30;
  xleg1 = 0.20;
  xleg2 = xleg1 + 0.25;
  yleg2 = 0.87;
  yleg1 = yleg2 - dyleg;
  TLegend* pleg = new TLegend(xleg1, yleg1, xleg2, yleg2);
  pleg->SetBorderSize(0);
  pleg->SetFillStyle(0);
  for ( unsigned int ixx=0; ixx<nxx; ++ixx ) {
    unsigned int npt = qcx[ixx].size();
    if ( qcx[ixx].size() > 0 ) {
      qgr[ixx] = new TGraph(npt, &qcx[ixx].front(), &qcy[ixx].front());
      qgr[ixx]->SetMarkerStyle(qmrk[ixx]);
      qgr[ixx]->SetMarkerColor(qcol[ixx]);
      cout << myname << setw(14) << slab[ixx] << " point count is " << npt << endl;
      pleg->AddEntry(qgr[ixx], slab[ixx].c_str(), "p");
    }
  }
  hnam = "haxqcor_" + dslist;
  sshtitl.str("");
  sshtitl << dslist << " quality correlation for the same chip; Q_{1}; Q_{2}";
  htitl = sshtitl.str();
  TH2* phaxq = new TH2F(hnam.c_str(), htitl.c_str(), 10, 0, 1, 10, 0, 1);
  phaxq->SetStats(0);
  pcan = new TCanvas;
  pcan->SetRightMargin(0.03);
  phaxq->Draw();
  TLine* plq = new TLine(0,0,1,1);
  plq->SetLineStyle(2);
  plq->Draw();
  pleg->Draw();
  for ( unsigned int ixx=0; ixx<nxx; ++ixx ) {
    TGraph* pgr = qgr[ixx];
    if ( pgr ) pgr->Draw("p");
  }
  fname = "chipQualityQCorrelation_" + dslist + ".png";
  if ( pcan != nullptr ) pcan->Print(fname.c_str());
  // Log summary.
  cout << "N80 counts:" << endl;
  Index csum = 0;
  for ( Index count=16; count<=16; --count ) {
    Index nchan = n80count[count];
    csum += nchan;
    cout << setw(4) << count << ":" << setw(5) << nchan << setw(6) << csum << endl;
    if ( count == 0 ) break;
  }
  cout << "Chip/Sample count: " << metricPrd.size() << "/" << ssams.size() << endl;
  cout << "  Chip count: " << nchip << endl;
  if ( pmets != nullptr ) {
    *pmets = metricPrd;
  }
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
