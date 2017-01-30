// AdcHist.cxx

#include "AdcHist.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TH2F.h"

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::ostringstream;
using std::getline;

typedef unsigned int Index;

//**********************************************************************

namespace {

bool stuck(Index iadc) {
  if ( iadc%64 == 0 ) return true;
  Index iadcp = iadc + 1;
  if ( iadcp%64 == 0 ) return true;
  return false;
}

};  // end unnamed namespace 

//**********************************************************************

AdcHist::AdcHist(string ssam, int chan, double cfac)
: pfit(nullptr), fitVinPerAdc(0.0), fitped(0.0) {
  const string myname = "AdcHist::ctor: ";
  const string stopdir = "/users/davidadams/data/dune/adc";
  string fname;
  string dirname;
  double zmax = 0;
  double dmax = 20;
  Index nd = 40;
  double ndmax = 10*dmax;
  double smax = 0.1*dmax;
  double gmin = 1.0;
  double gmax = 4.0;
  double lmin = 0.2;
  double lmax = 0.6;
  const string aschanold[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};
  const string aschan[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
  string schan = aschanold[chan];
  double vinmin = 0.0;
  double vinmax = 1600.0;
  double vinfitmax = 1700;
  Index iadcfitmin = 65;
  Index iadcfitmax = 4095;
  bool usestuck = false;
  double nomVinPerAdc = 1.0;
  double nomped = 0.0;
  bool isRaw = false;
  double dvdt = 144.0; // mV/sec
  double fsam = 2.e6; // Hz
  Index nadc = 4096;
  Index adcmax = nadc;
  Index npadc = 4100;
  Index padcmax = npadc;
  int pvinmin = -100;
  int pvinmax = 1900;
  Index npvin = pvinmax - pvinmin;
  adcUnderflow = 64;  // This and below are considered underflow
  string schanPrefix = "_chn";  // We will look for a file name with this followed by channel.
  Index bad = 0;
  // The following should either put the file name in fname or the directory name in dirname.
  // In the latter case, the file is found by searching for schanPrefix followed by aschan[chan];
  cout << endl;
  cout << "Sample " << ssam << " channel " << chan << endl;
  if ( ssam == "201610_vstar" ) {
    fname = stopdir + "/201610/VSTAR_1S/LN2_vstar_1s_200MHz_vstar_adc_09_28_2016_15_03_35/";
    fname += "LN2_vstar_1s_200MHz_vstar_adc_09_28_2016_15_03_35_chn" + aschanold[chan] + ".csv";
    nomVinPerAdc = 1.0/2.346;
  } else if ( ssam == "201610_p1" ) {
    fname = stopdir + "/201610/P1_ADC_DIE1/LN2_die1_200MHz_p1_adc_09_27_2016_19_18_27/";
    fname += "LN2_die1_200MHz_p1_adc_09_27_2016_19_18_27_chn" + aschanold[chan] + ".csv";
    nomVinPerAdc = 1.0/3.033;
  } else if ( ssam == "201611_p1_clkint" ) {
    string smin[16] = {"48", "48", "49", "49", "50", "50", "50", "51",
                       "51", "51", "52", "52", "52", "53", "53", "53"};
    fname = stopdir + "/201611/P1_die1/intClk_full_LN_2MHz/CSV/";
    fname += "P1_die1_intClk_full_LN_2MHz_chn" + aschan[chan] + "_11_02_19_" + smin[chan] + "_data.csv";
    vinmin = -50.0;
    vinmax = 1750;
    nomVinPerAdc = 0.343;
  } else if ( ssam == "201611_p1_clklvd" ) {
    string smin[16] = {"11", "11", "12", "12", "12", "13", "13", "13",
                       "14", "14", "14", "15", "15", "15", "16", "16"};
    fname = stopdir + "/201611/P1_die1/extClk_LVdefault_full_LN_2MHz/CSV/";
    fname += "P1_die1_extClk_LVdefault_full_LN_2MHz_chn" + aschan[chan] + "_11_02_20_" + smin[chan] + "_data.csv";
    vinmin = -50.0;
    vinmax = 1750;
    nomVinPerAdc = 1.0/2.933;
  } else if ( ssam == "201611_p1_clkfac" ) {
    string smin[16] = {"23", "23", "23", "24", "24", "24", "25", "25",
                       "25", "26", "26", "27", "27", "27", "28", "28"};
    fname = stopdir + "/201611/P1_die1/extClk_factory_full_LN_2MHz/CSV/";
    fname += "P1_die1_extClk_factory_full_LN_2MHz_chn" + aschan[chan] + "_11_02_20_" + smin[chan] + "_data.csv";
    vinmin = -50.0;
    vinmax = 1750;
    nomVinPerAdc = 0.342;
  } else if ( ssam == "201611_p1_old" ) {
    string smin[16] = {"17", "18", "18", "19", "19", "19", "20", "20",
                       "20", "21", "21", "21", "22", "22", "22", "23"};
    fname = stopdir + "/201611/P1_old/LN_2MHz_full/CSV/";
    fname += "P1_die1_extClk_factory_full_LN_2MHz_chn" + aschan[chan] + "_11_02_20_" + smin[chan] + "_data.csv";
    vinmin = -50.0;
    vinmax = 1750;
  } else if ( ssam == "201611_vstar" ) {
    string smin[16] = {"20", "20", "21", "21", "22", "22", "22", "23",
                       "23", "23", "24", "24", "24", "25", "25", "25"};
    fname = stopdir + "/201611/Vstar_old/LN_2MHz/CSV/";
    fname += "Vstar_B-2_ADC6_LN_2MHz_chn" + aschan[chan] + "_10_28_17_" + smin[chan] + "_data.csv";
    vinmin = -50.0;
    vinmax = 1750;
    nomVinPerAdc = 1.0/2.316;
  } else if ( ssam == "201612_ltc2314" ) {
    if ( chan!=0 && chan!=1 && chan !=8 && chan!=11 ) return;
    fname = stopdir + "/201612/ltc2314/LTC2314_raw_data/";
    fname += "ch" + aschan[chan] + "_360v_ltc2314_1_LN2_2M_006Hz_ramp_n01_23_sps_sps.csv";
    vinmin = -100.0;
    vinmax = 2300;
    nomVinPerAdc = 1.0/2.316;
    nadc = 4*nadc;
    adcmax = nadc;
    padcmax = 4*npadc;
    isRaw = true;
    iadcfitmin = 0;
    iadcfitmax = 16383;
    usestuck = true;
    nomVinPerAdc = 0.1151;
    adcUnderflow = 0;
    adcOverflow = nadc - 1;
  } else if ( ssam.substr(0, 10) == "201612_p1-" ) {     // 201612_p1-CB_clkTTT_FFFF_f3X
    string subdir;
    string schp;
    if ( ssam.size() == 28 ) {
      schp = ssam.substr(10,1);
      string sbos = ssam.substr(11,1);
      string sclk = ssam.substr(13,6);
      string sful = ssam.substr(20,4);
      string sf3x = ssam.substr(25,3);
      subdir = "LN2_2M_";
      if      ( sbos == "b" ) subdir += "brd";
      else if ( sbos == "s" ) subdir += "skt";
      else bad += 10;
      subdir += "_P1_" + schp;
      string sclksuf;
      string sfield = "_01";  // external
      if ( sclk == "clkint" ) sfield = "_00";
      else if ( sclk == "clkex1" ) sclksuf = "_clk1";
      else if ( sclk == "clkex2" ) sclksuf = "_clk2";
      else if ( sclk == "clkex3" ) sclksuf = "_clk3";
      else bad += 100;
      subdir += sfield;
      sfield = "";
      if      ( sful == "full" ) subdir += "_0";
      else if ( sful == "part" ) subdir += "_1";
      else bad += 1000;
      if      ( sf3x == "f30" ) subdir += "_0";
      else if ( sf3x == "f31" ) subdir += "_1";
      else bad += 10000;
      subdir += sclksuf;
    } else bad = 1;
    if ( ! bad ) dirname = stopdir + "/201612/p1_adc_old_format/P1_" + schp + "/" + subdir;
    vinmin = -50.0;
    vinmax = 1750;
    nomVinPerAdc = 1.0;
  } else if ( ssam.substr(0, 6) == "201701" ) {     // 201701_AA  AA=die # (00, 02, 03, ...)
    string subdir;
    string schp;
    if ( ssam.size() == 9 ) {
      schp = ssam.substr(7,2);
      dirname = stopdir + "/201701/P1_S7_" + schp;
      vinmin = -300.0;
      vinmax = 1700;
      nomVinPerAdc = 1.0;
      schanPrefix = "LN_2MHz_chn";
      isRaw = true;
      dvdt = 200.0;
      nomVinPerAdc = 0.34;
    } else bad = 1;
  } else {
    bad = 999;
  }
  if ( fname.size() == 0 ) {
    if ( dirname.size() == 0 ) {
      cout << myname << "Invalid sample name: " << ssam << endl;
      cout << myname << "Bad flag: " << bad << endl;
      return;
    }
    FileDirectory filedir(dirname);
    string spat = schanPrefix + aschan[chan];
    FileDirectory::FileMap files = filedir.find(spat);
    if ( files.size() == 1 ) {
      fname = filedir.dirname + "/" + files.begin()->first;
    } else {
      cout << myname << "Unable to find file with pattern " << spat
           << " in directory " << filedir.dirname << endl;
      return;
    }
  }
  if ( cfac > 0.0 ) nomVinPerAdc = cfac;
  cout << myname << "File: " << fname << endl;
  string stitle = ssam + " channel " + schan;
  string hnambase = "h" + ssam + "_" + schan + "_";
  string hnamf = hnambase + "fit";
  string hnamc = hnambase + "cor";
  string hnamd = hnambase + "dif";
  string hnamn = hnambase + "dfn";
  string hnamg = hnambase + "gai";
  string hnaml = hnambase + "lga";
  string hnamm = hnambase + "fmm";
  string hnams = hnambase + "fms";
  string hnamsg = hnambase + "fmsg";
  string hnamsb = hnambase + "fmsb";
  string hnamr = hnambase + "fmr";
  string hnamdn = hnambase + "fmdn";
  string hnamdr = hnambase + "fmdr";
  string hnamds = hnambase + "fmds";
  string hnamdsb = hnambase + "fmdsb";
  string stitlr = stitle + " fit RMS";
  string stitls = stitle + " fit sigma";
  string stitldsb = stitle + " stuck";
  phf = new TH2F(hnamf.c_str(), stitle.c_str(), npadc, 0, padcmax, npvin, pvinmin, pvinmax);
  phc = new TH2F(hnamc.c_str(), stitle.c_str(), npadc, 0, padcmax, npvin, pvinmin, pvinmax);
  phd = new TH2F(hnamd.c_str(), stitle.c_str(), npadc, 0, padcmax, 400, -dmax, dmax);
  phn = new TH2F(hnamn.c_str(), stitle.c_str(), npadc, 0, padcmax, 400, -ndmax, ndmax);
  phm = new TH1F(hnamm.c_str(), stitle.c_str(), npadc, 0, padcmax);
  phr = new TH1F(hnamr.c_str(), stitlr.c_str(), npadc, 0, padcmax);
  phs = new TH1F(hnams.c_str(), stitls.c_str(), npadc, 0, padcmax);
  phdn = new TH1F(hnamdn.c_str(), stitle.c_str(), nd, 0, ndmax);
  phdr = new TH1F(hnamdr.c_str(), stitle.c_str(), nd, 0, dmax);
  phds = new TH1F(hnamds.c_str(), stitle.c_str(), nd, 0, smax);
  phdsb = new TH1F(hnamdsb.c_str(), stitldsb.c_str(), nd, 0, smax);
  vector<TH1*> hists = {phf, phc, phd, phn};
  for ( TH1* ph : hists ) {
    ph->SetStats(0);
    ph->SetContour(20);
    if ( zmax > 0 ) ph->SetMaximum(zmax); 
    ph->GetXaxis()->SetTitle("ADC count");
  }
  vector<TH1*> dhists = {phdn, phdr, phds, phdsb};
  for ( TH1* ph : dhists ) {
    //ph->SetStats(0);
    ph->GetYaxis()->SetTitle("# ADC bins");
  }
  phdn->GetXaxis()->SetTitle("Nominal resolution [mV]");
  phdr->GetXaxis()->SetTitle("RMS [mV]");
  phds->GetXaxis()->SetTitle("Sigma [mV]");
  phdsb->GetXaxis()->SetTitle("Sigma [mV]");
  phc->GetYaxis()->SetTitle("Input level [mV]");
  phm->GetYaxis()->SetTitle("V_{in} diff mean [mV]");
  phr->GetYaxis()->SetTitle("V_{in} diff RMS [mV]");
  phs->GetYaxis()->SetTitle("V_{in} diff standard deviation [mV]");
  phm->SetStats(0);
  phd->SetStats(0);
  phr->SetStats(0);
  phs->SetStats(0);
  phm->SetMaximum(dmax);
  phm->SetMinimum(-dmax);
  phr->SetMaximum(dmax);
  phr->SetMinimum(0.0);
  phs->SetMaximum(0.1*dmax);
  phs->SetMinimum(0.0);
  phsg = dynamic_cast<TH1*>(phs->Clone(hnamsg.c_str()));
  phsb = dynamic_cast<TH1*>(phs->Clone(hnamsb.c_str()));
  ifstream fin(fname.c_str());
  Index nline = 0;
  Index nvin = 10*(vinmax-vinmin);
  vector<double> vvin(nvin, -5000);
  for ( Index ivin=0; ivin<nvin; ++ivin ) {
    vvin[ivin] = vinmin + 0.1*ivin;
  }
  // Fetch the count for each ADC-Vin bin
  vector<vector<Index>> vcount(nadc);
  Index maxvin = 0;
  Index fullcount = 0;
  if ( isRaw ) {
    cout << "Reading raw data." << endl;
    double dv = 0.0;
    double dt = 1.0/fsam;  // sec
    Index isam = 0;
    for ( vector<Index>& vec : vcount ) vec.resize(nvin);
    while ( fin && ! fin.eof() ) {
      double iadc = 0.0;
      double t = isam*dt;
      double vin = dvdt*t;
      Index ivin = vin/0.1;
      if ( ivin > nvin ) {
        cout << "Vin is too large: " << vin << endl;
        return;
      }
      string sline;
      getline(fin, sline);
      //cout << myname << "  ... " << isam << ": " << vin << ", " << sline << endl;
      istringstream ssline(sline);
      ssline >> iadc;
      if ( ! fin ) {
        //cout << myname << "File read failed." << endl;
        break;
      }
      ++vcount[iadc][ivin];
      //if ( iadc == 500 ) cout << myname << "  ... " << iadc << ", " << ivin << ": " << vcount[iadc][ivin] << endl;
      ++isam;
    }
    maxvin = nvin;
    nline = isam;
    fullcount = isam;
    cout << "# samples: " << isam << endl;
  } else {
    cout << "Reading table data." << endl;
    for ( Index iadc=0; iadc<nadc; ++iadc ) {
      vcount[iadc].resize(nvin, 0);
      string sline;
      getline(fin, sline);
      if ( ! fin ) {
        cout << myname << "File read failed." << endl;
        break;
      }
      istringstream ssline(sline);
      Index ivin;
      for ( Index ivin=0; ivin<nvin; ++ivin ) {
        ssline >> vcount[iadc][ivin];
        fullcount += vcount[iadc][ivin];
        if ( ! ssline ) {
          break;
        }
        if ( ivin >= maxvin ) maxvin = ivin+1;
        ssline.ignore(10, ',');
      }
      ++nline;
    }
  }
  double countPerVinBin = double(fullcount)/nadc;
  minCountForStats = 2 + countPerVinBin/1000;
  cout << myname << "# ADC: " << nadc << endl;
  cout << myname << "# Vin: " << nvin << endl;
  cout << myname << "# lines: " << nline << endl;
  cout << "Total count: " << fullcount << endl;
  cout << "Total count/nvin: " << fullcount/nvin << endl;
  cout << "Total count/nadc: " << fullcount/nadc << endl;
  cout << "Min count for stats: " << minCountForStats << endl;
  if ( zmax == 0 ) {
    zmax = 4*fullcount/nvin;
    cout << "zmax = " << zmax << endl;
    for ( TH1* ph : hists ) ph->SetMaximum(zmax); 
  }
  // Create the fit and response histograms.
  for ( Index iadc=0; iadc<nadc; ++iadc ) {
    for ( Index ivin=0; ivin<nvin; ++ivin ) {
      double vin = vvin[ivin];
      Index count = vcount[iadc][ivin];
      phc->Fill(iadc, vin, count);
      //if ( iadc == 500 && ivin < 700) cout << "XXX: " << iadc << ", " << vin << ": " << count << endl;
      if ( iadc >= iadcfitmin && iadc < iadcfitmax && vin < vinfitmax ) {
        Index rem = iadc%64;
        bool skip = false;
        if ( ! usestuck ) skip = rem == 0 || rem == 63;
        if ( ! skip ) phf->Fill(iadc, vin, count);
      }
    }
  }
  // Fit the response histogram.
  phf->Fit("pol1");
  pfit = phf->GetFunction("pol1");
  fitped = pfit->GetParameter(0);
  fitVinPerAdc = pfit->GetParameter(1);
  cout << "Fit gain: " << fitVinPerAdc << " mV/ADC, pedestal: " << fitped << " mV" << endl;
  ostringstream ssdif;
  ssdif.precision(3);
  ssdif << "(" << fitVinPerAdc << " ADC + " << fitped << ") - V_{in} [mV]";
  phd->GetYaxis()->SetTitle(ssdif.str().c_str());
  // Find the nominal pedestal.
  Index adc_nom = 750;
  nomped = fitped + (fitVinPerAdc - nomVinPerAdc)*adc_nom;
  cout << "Nominal gain: " << nomVinPerAdc << " mV/ADC, pedestal: " << nomped << " mV" << endl;
  ostringstream ssdifn;
  ssdifn.precision(3);
  ssdifn << "(" << nomVinPerAdc << " ADC + " << nomped << " ) - V_{in} [mV]";
  phn->GetYaxis()->SetTitle(ssdifn.str().c_str());
  // Fill the remaining histograms.
  for ( Index iadc=0; iadc<nadc; ++iadc ) {
    for ( Index ivin=0; ivin<nvin; ++ivin ) {
      double vin = vvin[ivin];
      double evin = fitped + iadc*fitVinPerAdc;
      double evin_nom = nomped + iadc*nomVinPerAdc;
      Index count = vcount[iadc][ivin];
      phd->Fill(iadc, evin - vin, count);
      phn->Fill(iadc, evin_nom - vin, count);
    }
  }
  // Fill diff stat histograms.
  for ( Index iadc=0; iadc<nadc; ++iadc ) {
    TH1* ph = hdiff(iadc);
    if ( ph == nullptr ) continue;
    double xm = -99999.;
    double xs = -99.;
    if ( ph->GetEntries() >= minCountForStats ) {
      xm = ph->GetMean();
      xs = ph->GetRMS();
    }
    bool isStuck = stuck(iadc);
    double xsg = isStuck ? 0 : xs;
    double xsb = !isStuck ? 0 : xs;
    double xr = sqrt(xm*xm+xs*xs);
    phm->SetBinContent(iadc+1, xm);
    phs->SetBinContent(iadc+1, xs);
    phsg->SetBinContent(iadc+1, xsg);
    phsb->SetBinContent(iadc+1, xsb);
    phr->SetBinContent(iadc+1, xr);
    if ( iadc > adcUnderflow && iadc < adcOverflow ) {
      bool bad = !usestuck && isStuck;
      phdr->Fill(xr);
      if ( bad ) {
        phdsb->Fill(xs);
      } else {
        phds->Fill(xs);
      }
    }
    ph = hdiffn(iadc);
    xm = ph->GetMean();
    xs = ph->GetRMS();
    xr = sqrt(xm*xm+xs*xs);
    if ( iadc > 64 && !isStuck ) {
      phdn->Fill(xr);
    }
  }
  return;
}

//**********************************************************************

TH1* AdcHist::hdiff(Index iadc) const {
  if ( phd == nullptr ) return nullptr;
  ostringstream sshnam;
  sshnam << phd->GetName() << "_adc" << iadc;
  string hnam = sshnam.str();
  Index bin = iadc + 1;
  TH1* ph = phd->ProjectionY(hnam.c_str(), bin, bin);
  return ph;
}

//**********************************************************************

TH1* AdcHist::hdiffn(Index iadc) const {
  if ( phn == nullptr ) return nullptr;
  ostringstream sshnam;
  sshnam << phn->GetName() << "_adc" << iadc;
  string hnam = sshnam.str();
  Index bin = iadc + 1;
  TH1* ph = phn->ProjectionY(hnam.c_str(), bin, bin);
  return ph;
}

//**********************************************************************

