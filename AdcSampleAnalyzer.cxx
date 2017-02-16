// AdcSampleAnalyzer.cxx

#include "AdcSampleAnalyzer.h"
#include <string>
#include <iostream>
#include <sstream>
#include "TH2F.h"

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;

using Name = AdcSampleReader::Name;
using Index = AdcSampleReader::Index;

//**********************************************************************

namespace {

bool sticky(Index iadc) {
  if ( iadc%64 == 0 ) return true;
  Index iadcp = iadc + 1;
  if ( iadcp%64 == 0 ) return true;
  return false;
}

};  // end unnamed namespace 

//**********************************************************************

AdcSampleAnalyzer::AdcSampleAnalyzer(Name ssam, Index chan, Index maxsam, double cfac)
: reader(ssam, chan, maxsam),
  pfit(nullptr), fitVinPerAdc(0.0), fitped(0.0) {
  const string myname = "AdcSampleAnalyzer::ctor: ";
  if ( reader.countTable().size() == 0 ) {
    cout << myname << "Unable to read data." << endl;
    return;
  }
  double zmax = 0;
  double dmax = 20;
  Index nd = 40;
  double ndmax = 10*dmax;
  double smax = 0.1*dmax;
  double gmin = 1.0;
  double gmax = 4.0;
  double lmin = 0.2;
  double lmax = 0.6;
  double vinfitmax = 1700;
  Index nadc = reader.nadc();
  Index iadcfitmin = nadc==4096 ? 65 : 1;
  Index iadcfitmax = nadc - 1;
  bool usestuck = false;
  double nomVinPerAdc = cfac == 0.0 ? reader.nomVinPerAdc() : cfac;
  double nomped = 0.0;
  Index adcmax = nadc;
  Index nvin = reader.nvin();
  Index npadc = 4100*nadc/4096;
  Index padcmax = npadc;
  int pvinmin = -100;
  int pvinmax = 1900;
  Index npvin = pvinmax - pvinmin;
  adcUnderflow = 64;  // This and below are considered underflow
  if ( ssam.find("ltc") != string::npos ) {
    usestuck = true;
    nomVinPerAdc = 0.1151;
    adcUnderflow = 0;
  }
  Index nsample = reader.nsample();
  calib.chip = reader.chip();
  calib.chan = chan;
  ostringstream sschan;
  sschan << chan;
  string schan = sschan.str();
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
  phdw = new TH2F(hnamd.c_str(), stitle.c_str(), npadc, 0, padcmax, 400, -10*dmax, 10*dmax);
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
    ph->SetLineWidth(2);
  }
  vector<TH1*> dhists = {phdn, phdr, phds, phdsb, phdw};
  for ( TH1* ph : dhists ) {
    //ph->SetStats(0);
    ph->GetYaxis()->SetTitle("# ADC bins");
    ph->SetLineWidth(2);
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
  phdw->SetStats(0);
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
  double countPerVinBin = double(nsample)/nadc;
  minCountForStats = 2 + countPerVinBin/1000;
  cout << myname << "# ADC: " << nadc << endl;
  cout << myname << "# Vin: " << nvin << endl;
  cout << "Min count for stats: " << minCountForStats << endl;
  if ( zmax == 0 ) {
    zmax = 4*nsample/nvin;
    cout << myname << "zmax = " << zmax << endl;
    for ( TH1* ph : hists ) ph->SetMaximum(zmax); 
  }
  // Create the fit and response histograms.
  Index nfill = 0;
  Index nskip = 0;
  for ( Index iadc=0; iadc<nadc; ++iadc ) {
    for ( Index ivin=0; ivin<nvin; ++ivin ) {
      double vin = reader.vin(ivin);
      Index count = reader.countTable()[iadc][ivin];
      phc->Fill(iadc, vin, count);
      //if ( iadc == 500 && ivin < 700) cout << myname << "XXX: " << iadc << ", " << vin << ": " << count << endl;
      if ( iadc >= iadcfitmin && iadc < iadcfitmax && vin < vinfitmax ) {
        Index rem = iadc%64;
        bool skip = false;
        if ( ! usestuck ) skip = rem == 0 || rem == 63;
        if ( skip ) {
          ++nskip;
        } else {
          ++nfill;
          phf->Fill(iadc, vin, count);
        }
      }
    }
  }
  cout << myname << "Response fit histogram nfill=" << nfill << ", nskip=" << nskip << endl;
  if ( nfill == 0 ) return;
  // Fit the response histogram.
  cout << myname << "Fitting..." << endl;
  phf->Fit("pol1", "Q");
  pfit = phf->GetFunction("pol1");
  fitped = pfit->GetParameter(0);
  fitVinPerAdc = pfit->GetParameter(1);
  calib.gain = fitVinPerAdc;
  calib.offset = fitped;
  cout << myname << "Fit gain: " << fitVinPerAdc << " mV/ADC, pedestal: " << fitped << " mV" << endl;
  ostringstream ssdif;
  ssdif.precision(3);
  ssdif << "(" << fitVinPerAdc << " ADC + " << fitped << ") - V_{in} [mV]";
  phd->GetYaxis()->SetTitle(ssdif.str().c_str());
  phdw->GetYaxis()->SetTitle(ssdif.str().c_str());
  // Find the nominal pedestal.
  Index adc_nom = 750;
  nomped = fitped + (fitVinPerAdc - nomVinPerAdc)*adc_nom;
  cout << myname << "Nominal gain: " << nomVinPerAdc << " mV/ADC, pedestal: " << nomped << " mV" << endl;
  ostringstream ssdifn;
  ssdifn.precision(3);
  ssdifn << "(" << nomVinPerAdc << " ADC + " << nomped << " ) - V_{in} [mV]";
  phn->GetYaxis()->SetTitle(ssdifn.str().c_str());
  // Fill the remaining histograms.
  for ( Index iadc=0; iadc<nadc; ++iadc ) {
    for ( Index ivin=0; ivin<nvin; ++ivin ) {
      double vin = reader.vin(ivin);
      double evin = fitped + iadc*fitVinPerAdc;
      double evin_nom = nomped + iadc*nomVinPerAdc;
      Index count = reader.countTable()[iadc][ivin];
      phd->Fill(iadc, evin - vin, count);
      phdw->Fill(iadc, evin - vin, count);
      phn->Fill(iadc, evin_nom - vin, count);
    }
  }
  // Fill diff stat histograms.
  calib.calMeans.resize(nadc);
  calib.calRmss.resize(nadc);
  calib.calCounts.resize(nadc);
  for ( Index iadc=0; iadc<nadc; ++iadc ) {
    TH1* ph = hdiffcalib(iadc);
    if ( ph == nullptr ) continue;
    double xm = -99999.;
    double xs = -99.;
    unsigned int count = ph->Integral();
    if ( count >= minCountForStats ) {
      xm = ph->GetMean();
      xs = ph->GetRMS();
    }
    double xm0 = fitVinPerAdc*iadc + fitped;
    calib.calCounts[iadc] = count;
    calib.calMeans[iadc] = xm + xm0;
    calib.calRmss[iadc] = xs;
    bool isStuck = sticky(iadc);
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

TH1* AdcSampleAnalyzer::hcalib(Index iadc) const {
  if ( phc == nullptr ) return nullptr;
  ostringstream sshnam;
  sshnam << phc->GetName() << "_adc" << iadc;
  string hnam = sshnam.str();
  Index bin = iadc + 1;
  TH1* ph = phc->ProjectionY(hnam.c_str(), bin, bin);
  int nbin = ph->GetNbinsX();
  ostringstream sshtitl;
  sshtitl << phc->GetTitle() << " bin " << iadc;
  string shtitl = sshtitl.str();
  ph->SetTitle(shtitl.c_str());
  return ph;
}

//**********************************************************************

TH1* AdcSampleAnalyzer::hdiff(Index iadc) const {
  if ( phd == nullptr ) return nullptr;
  ostringstream sshnam;
  sshnam << phd->GetName() << "_adc" << iadc;
  string hnam = sshnam.str();
  Index bin = iadc + 1;
  TH1* ph = phd->ProjectionY(hnam.c_str(), bin, bin);
  ostringstream sshtitl;
  sshtitl << phd->GetTitle() << " bin " << iadc;
  string shtitl = sshtitl.str();
  ph->SetTitle(shtitl.c_str());
  return ph;
}

//**********************************************************************

TH1* AdcSampleAnalyzer::hdiffn(Index iadc) const {
  if ( phn == nullptr ) return nullptr;
  ostringstream sshnam;
  sshnam << phn->GetName() << "_adc" << iadc;
  string hnam = sshnam.str();
  Index bin = iadc + 1;
  TH1* ph = phn->ProjectionY(hnam.c_str(), bin, bin);
  ostringstream sshtitl;
  sshtitl << phn->GetTitle() << " bin " << iadc;
  string shtitl = sshtitl.str();
  ph->SetTitle(shtitl.c_str());
  return ph;
}

//**********************************************************************

TH1* AdcSampleAnalyzer::hdiffcalib(Index iadc) const {
  bool zeroerrs = 0;
  if ( phd == nullptr ) return nullptr;
  TH1* ph = hdiff(iadc);
  unsigned int nbin = ph->GetNbinsX();
  if ( ph->GetBinContent(0) || ph->GetBinContent(nbin+1) ) {
    ostringstream sshnam;
    sshnam << phdw->GetName() << "_adc" << iadc;
    string hnam = sshnam.str();
    Index bin = iadc + 1;
    ph = phdw->ProjectionY(hnam.c_str(), bin, bin);
    ostringstream sshtitl;
    sshtitl << phdw->GetTitle() << " bin " << iadc;
    string shtitl = sshtitl.str();
    ph->SetTitle(shtitl.c_str());
  }
  if ( zeroerrs ) {
    for ( int ich=0; ich<=ph->GetNbinsX()+1; ++ich ) {
      ph->SetBinError(ich, 0.0);
    }
  }
  return ph;
}

//**********************************************************************

AdcSampleAnalyzer::AdcVoltageResponseVector&
AdcSampleAnalyzer::evaluateVoltageReponses(double vmin, double vmax, Index nv) {
  if ( nv == 0 ) return voltageResponses;
  if ( vmin >= vmax ) return voltageResponses;
  float dv = (vmax - vmin)/nv;
  float v1 = vmin;
  float v2 = v1 + dv;
  AdcChannelId id(reader.chip(), reader.channel());
  for ( Index iv=0; iv<nv; ++iv ) {
    if ( iv > 0 ) {
      v1 = v2;
      v2 += dv;
    }
    voltageResponses.push_back(AdcVoltageResponse(id, v1, v2));
  }
  // Still must add code to fill voltage responses!!
  return voltageResponses;
}

//**********************************************************************
