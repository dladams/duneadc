using SampleMetricMap = map<string, double>;
using SampleIndexMap = map<string, Index>;
using StringVector = vector<string>;
using StringStringMap = map<string,string>;
using ChipSample = std::pair<Index, string>;
using ValueChipSample = std::pair<double, ChipSample>;
using ChipMetricMap = map<Index, ValueChipSample>;
using DatasetChipMetricMap = map<string, ChipMetricMap>;
using RankMap = multimap<double, ChipSample>;
using ChanValMap = map<string, std::vector<double>>;
using DoubleVector = vector<double>;
using DoubleVectorVector = vector<DoubleVector>;

void writePython(string name, const RankMap& chips);

string sentrycount(TH1* ph) {
  Index nbin = ph->GetNbinsX();
  Index bin1 = nbin/2 + 1;
  Index nhalf = ph->Integral(bin1, nbin+1);
  Index ntot = ph->GetEntries();
  ostringstream sslab;
  sslab << nhalf << "/" << ntot;
  return sslab.str();
}

// datasetString - colon-separated list of datasets
//                 Sample names  for dataset DST are read from DST.txt and perf data from perf_DST.root
// dslist - if not blank and dslist.txt is not empty, only the samples in that file are used
TH1* rankChips(string datasetString="PDTS:CETS", string dslist ="", SampleMetricMap* pmets =nullptr) {
  string myname = "rankChips: ";
  string::size_type ipos = 0;
  vector<string> datasets;
  while ( ipos != string::npos ) {
    string::size_type jpos = datasetString.find(":", ipos);
    datasets.push_back(datasetString.substr(ipos, jpos-ipos));
    ipos = jpos;
    if ( ipos != string::npos ) ++ipos;
  }
  cout << myname << "Datasets used in performance evaluation:" << endl;
  string sdsts;
  for ( string dst : datasets ) {
    cout << myname << "  " << dst << endl;
    if ( sdsts.size() ) sdsts += "-";
    sdsts += dst;
  }
  fnamedsts = dslist + "_" + sdsts; // Suffix for filenames.
  cout << myname << "Dataset list: " << dslist << endl;
  SampleIndexMap sampleChip;
  SampleIndexMap sampleDay;
  RankMap rankedChipsPrd;
  SampleMetricMap metricPrd;
  SampleMetricMap metricAvg;
  SampleMetricMap metricLow;
  SampleIndexMap metricLo2;
  SampleMetricMap metricN80;
  ChipMetricMap chipMetricPrd;                // Qmax for each chip.
  DatasetChipMetricMap datasetChipMetricPrd;  // Qmax for each chip in each dataset.
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
  vector<string> histDatasetNames;
  if ( ndst == 0 ) {
    cout << myname << "ERROR: No datasets specified." << endl;
    return nullptr;
  }
  Index nhst = 1;
  if ( ndst > 1 ) nhst += ndst;
  // Create Qmax histos.
  for ( Index ihst=0; ihst<nhst; ++ihst ) {
    string dsname = ihst==0 ? "all" : datasets[ihst-1];
    if ( ihst == 0 && ndst == 1 ) dsname = datasets[0];
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
  vector<string> ssamsin;
  if ( dslist.size() ) {
    string dsfname = dslist + ".txt";
    ifstream dsfile(dsfname.c_str());
    if ( ! dsfile ) {
      cout << myname << "Dataset file not found: " << dsfname << endl;
      return nullptr;
    }
    while ( dsfile ) {
      string ssam;
      dsfile >> ssam;
      if ( dsfile.eof() ) break;
      ssamsin.push_back(ssam);
    }
  }
  Index wsam = 0;
  // Read the samples for each dataset.
  vector<string> ssams;
  std::vector<vector<string>> datasetSamples(ndst);
  bool dbg = false;
  unsigned int readcountTot = 0;
  map<string,Index> datasetIndex;    // Dataset index for each sample. ndst if not found.
  for ( Index idst=0; idst<ndst; ++idst ) {
    string dataset = datasets[idst];
    string dsfname = dataset + ".txt";
    ifstream dsf(dsfname.c_str());
    if ( ! dsf ) {
      cout << myname << "Dataset description file not found: " << dsfname << endl;
      return nullptr;
    }
    unsigned int readcount = 0;
    while ( dsf ) {
      string ssam;
      dsf >> ssam;
      if ( dsf.eof() ) break;
      ++readcount;
      bool keep = true;
      if ( ssamsin.size() && find(ssamsin.begin(), ssamsin.end(), ssam) == ssamsin.end() ) keep = false;
      if ( keep ) {
        if ( datasetIndex.find(ssam) != datasetIndex.end() ) {
           cout << myname << "ERROR: Sample " << ssam << " is in two datasets: "
                << datasetIndex[ssam] << " and " << dataset << endl;
           return nullptr;
        }
        datasetIndex[ssam] = idst;
        datasetSamples[idst].push_back(ssam);
        sampleDataset[ssam] = dataset;
        ssams.push_back(ssam);
        Index lsam = ssam.size();
        if ( lsam > wsam ) wsam = lsam;
      } else {
        if ( dbg ) cout << myname << "Dropping " << ssam << endl;
      }
    }
    cout << myname << "Dataset " << dataset << " keep/read: " << datasetSamples[idst].size() << "/" << readcount << endl;
    readcountTot += readcount;
  }
  cout << myname << "TOTAL keep/read: " << ssams.size() << "/" << readcountTot << endl;
  wsam += 4;
  bool first = true;
  // Process the data.
  // Loop over datasets.
  Index ncha = 0;
  for ( unsigned int idst=0; idst<ndst; ++idst ) {
    string dataset = datasets[idst];
    // Loop over samples in each dataset.
    for ( string ssam : datasetSamples[idst] ) {
      std::unique_ptr<AdcChipMetric> pacm(new AdcChipMetric(dataset, ssam));
      int estat = pacm->evaluate();
      if ( estat != 0 ) {
        cout << myname << "Unable to find performance for " << ssam << endl;
        continue;
      }
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
      Index time = acm.time();
      TDatime rtime(time);
      Index ifday = 10000*rtime.GetYear() + 100*rtime.GetMonth() + rtime.GetDay();
      sampleDay[ssam] = ifday;
      cout << myname << "Sample " << ssam << " (chip " << chip << ", day " << ifday << ")" << endl;
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
      // Update Qmax for all.
      auto iprd = chipMetricPrd.find(chip);
      bool insert = iprd == chipMetricPrd.end();
      if ( ! insert ) {
        string oldsam = iprd->second.second.second;
        double oldeff = metricPrd[oldsam];
        insert = effprd > oldeff;
      }
      if ( insert ) chipMetricPrd[chip] = valprd;
      // Update Qmax for this dataset.
      auto& dcMetricPrd = datasetChipMetricPrd[dataset];
      iprd = dcMetricPrd.find(chip);
      insert = iprd == dcMetricPrd.end();
      if ( ! insert ) {
        string oldsam = iprd->second.second.second;
        double oldeff = metricPrd[oldsam];
        insert = effprd > oldeff;
      }
      if ( insert ) dcMetricPrd[chip] = valprd;
      //
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
  string outsumName = "rank_" + fnamedsts + ".txt";
  string outsum2Name = "rank_" + fnamedsts + "_long.txt";
  ofstream outsum(outsumName.c_str());
  ofstream outsum2(outsum2Name.c_str());
  ostringstream outl;
  int wds = 7;
  int wday = 9;
  int wval = 8;
  bool showDatasetMetrics = true;
  for ( string dataset : datasets ) if ( dataset.size() > wds ) wds = dataset.size();
  wds += 2;
  outl << setw(5) << "Rank" << setw(6) << "Chip" << setw(wval) << "Q" << setw(4) << "N80";
  if ( ndst > 1 ) outl << setw(wds) << "Dataset";
  outl << setw(wday) << "Day";
  if ( showDatasetMetrics ) {
    for ( string dataset : datasets ) {
      string lab = "Q" + dataset.substr(0,wval-2);
      outl << setw(wval) << lab;
    }
  }
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
    outl << setw(wday) << sampleDay[ssam];
    if ( showDatasetMetrics ) {
      for ( string dataset : datasets ) {
        auto iprd = datasetChipMetricPrd[dataset].find(chip);
        if ( iprd == datasetChipMetricPrd[dataset].end() ) {
          outl << setw(wval) << "";
        } else {
          string ssam = iprd->second.second.second;
          double qdmax = metricPrd[ssam];
          outl << setw(wval) << qdmax;
        }
      }
    }
    outsum << outl.str() << endl;
    // Add  dataset metrics.
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
  string ofname = "rank_" + fnamedsts + ".py";
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
  TH1* ph0 = hists[0];
  string htitl = dslist + " ADC chip quality (" + sentrycount(ph0) + " chips)";
  ph0->SetTitle(htitl.c_str());
  dyleg = 0.02 + 0.04*nhst;
  double xleg1 = 0.40;
  double xleg2 = xleg1 + 0.30;
  double yleg2 = 0.88;
  double yleg1 = yleg2 - dyleg;
  htitl = sshtitl.str();
  ph0->SetLineWidth(3);
  double ymax = ph0->GetMaximum();
  bool doymax = false;
  double ybinmax = 0.0;
  for ( ibin=4; ibin<=ph0->GetNbinsX(); ++ibin ) {
    double ybin = ph0->GetBinContent(ibin);
    if ( ybin > ybinmax ) ybinmax = ybin;
  }
  if ( ybinmax < 0.7*ymax ) {
    ymax = int(1.3*ybinmax+0.5);
    doymax = true;
  }
  if ( ymax < 10.0 ) {
    ymax = 10.0;
    doymax = true;
  }
  ph0->SetMinimum(0.0);
  if ( doymax ) ph0->SetMaximum(ymax);
  string fnameQuality = "chipQuality_" + fnamedsts + ".png";
  // Draw quality overlaying chip lists.
  vector<int> cols;
  vector<int> stys;
  vector<int> wids;
  for ( string dst : datasets ) {
    if ( dst == "PDTS2" ) {
      cols.push_back(46);
      stys.push_back(3);
      wids.push_back(3);
    } else if ( dst == "PDTS1" ) {
      cols.push_back(1);
      stys.push_back(1);
      wids.push_back(1);
    } else if ( dst == "PDTStry" ) {
      cols.push_back(kMagenta+2);
      stys.push_back(1);
      wids.push_back(1);
    } else if ( dst == "CETS" ) {
      cols.push_back(kGreen + 3);
      stys.push_back(2);
      wids.push_back(2);
    } else {
      cols.push_back(1);
      stys.push_back(1);
      wids.push_back(1);
    }
  }
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
      ostringstream sslab;
      sslab << slabs[ihst] << " (" << sentrycount(ph) << " chips)";
      string slab = sslab.str();
      if ( ihst > 0 ) {
        ph->SetLineColor(cols[ihst-1]);
        ph->SetLineStyle(stys[ihst-1]);
        ph->SetLineWidth(wids[ihst-1]);
      }
      ph->Draw("same");
      pleg->AddEntry(ph, slab.c_str(), "l");
    }
    pleg->Draw();
    if ( pcan != nullptr ) pcan->Print(fnameQuality.c_str());
    // Change name for combined plot if we also have multi-list plot.
    fnameQuality = "chipQualityCombined_" + fnamedsts + ".png";
  }
  // Draw combined quality overlaying chip lists.
  pcan = new TCanvas;
  pcan->SetRightMargin(0.03);
  TH1* ph = hists[0];
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
  fname = "chipQualityAvgCorrelation_" + fnamedsts + ".png";
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
  fname = "chipQualityLowCorrelation_" + fnamedsts + ".png";
  if ( pcan != nullptr ) pcan->Print(fname.c_str());
  // Plot q correlations: Q vs Qchip_max
  const int i11 = 0;
  const int i22 = 1;
  const int iCC = 2;
  const int i2C = 3;
  const int i21 = 4;
  const int iC1 = 5;
  const int nxx = 6;
  string ssam0 = "PDTS1";
  if ( datasets[0] == "PDTStry" ) ssam0 = datasets[0];
  string ssam1 = "PDTS2";
  string ssam2 = "CETS";
  string ssam00 = ssam0 + " " + ssam0;
  string ssam11 = ssam1 + " " + ssam1;
  string ssam22 = ssam2 + " " + ssam2;
  string ssam12 = ssam1 + " " + ssam2;
  string ssam10 = ssam1 + " " + ssam0;
  string ssam20 = ssam2 + " " + ssam0;
  StringVector slab{ssam00, ssam11, ssam22, ssam12, ssam10, ssam20};
  vector<int> qmrk = {2, 2, 2,  4, 26, 32};
  int col21 = 28; 
  vector<int> qcol = {cols[0], cols[1], cols[2], col21, cols[1], cols[2]};
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
          flip |= dst2 == ssam1;
          flip |= dst1 == ssam0;
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
          cout << myname << "Unable to find dataset pair index for " << lab << endl;
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
  dyleg = 0.20;
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
  fname = "chipQualityQCorrelation_" + fnamedsts + ".png";
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
