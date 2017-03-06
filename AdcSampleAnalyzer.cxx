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
using FloatVector = AdcVoltagePerformance::FloatVector;

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

AdcSampleAnalyzer::AdcSampleAnalyzer(Name ssam, Index chan, string adatasetCalib, Index maxsam, double cfac)
: reader(ssam, chan, maxsam),
  datasetCalib(adatasetCalib),
  pfit(nullptr), fitVinPerAdc(0.0), fitped(0.0) {
  const string myname = "AdcSampleAnalyzer::ctor: ";
  if ( reader.countTable().size() == 0 ) {
    cout << myname << "Unable to read data." << endl;
    return;
  }
  double zmax = 0;
  double dmax = 20;
  Index nd = 40;
  double wdmax = 10*dmax;
  double smax = 0.1*dmax;
  double gmin = 1.0;
  double gmax = 4.0;
  double lmin = 0.2;
  double lmax = 0.6;
  Index nadc = reader.nadc();
  iadcfitmin = nadc==4096 ? 128 : 1;
  iadcfitmax = nadc - 1;
  vinfitmin = 0.0;
  vinfitmax = 1600.0;
  fitusestuck = false;
  nomVinPerAdc = cfac == 0.0 ? reader.nomVinPerAdc() : cfac;
  nomped = 0.0;
  Index adcmax = nadc;
  Index nvin = reader.nvin();
  Index npadc = 4100*nadc/4096;
  Index padcmax = npadc;
  int pvinmin = reader.vinmin();
  int pvinmax = reader.vinmax();
  Index npvin = pvinmax - pvinmin;
  adcUnderflow = 64;  // This and below are considered underflow
  if ( ssam.find("ltc") != string::npos ) {
    fitusestuck = true;
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
  string hnamdw = hnambase + "difw";
  string hnamn = hnambase + "dfn";
  string hnamnw = hnambase + "dfnw";
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
  phdw = new TH2F(hnamdw.c_str(), stitle.c_str(), npadc, 0, padcmax, 400, -wdmax, wdmax);
  phn = new TH2F(hnamn.c_str(), stitle.c_str(), npadc, 0, padcmax, 400, -dmax, dmax);
  phnw = new TH2F(hnamnw.c_str(), stitle.c_str(), npadc, 0, padcmax, 400, -wdmax, wdmax);
  phm = new TH1F(hnamm.c_str(), stitle.c_str(), npadc, 0, padcmax);
  phr = new TH1F(hnamr.c_str(), stitlr.c_str(), npadc, 0, padcmax);
  phs = new TH1F(hnams.c_str(), stitls.c_str(), npadc, 0, padcmax);
  phdn = new TH1F(hnamdn.c_str(), stitle.c_str(), nd, 0, wdmax);
  phdr = new TH1F(hnamdr.c_str(), stitle.c_str(), nd, 0, dmax);
  phds = new TH1F(hnamds.c_str(), stitle.c_str(), nd, 0, smax);
  phdsb = new TH1F(hnamdsb.c_str(), stitldsb.c_str(), nd, 0, smax);
  vector<TH1*> hists = {phf, phc, phd, phn, phnw};
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
  phn->SetStats(0);
  phnw->SetStats(0);
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
      if ( iadc > iadcfitmin && iadc < iadcfitmax && vin > vinfitmin && vin < vinfitmax ) {
        Index rem = iadc%64;
        bool skip = false;
        if ( ! fitusestuck ) skip = rem == 0 || rem == 63;
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
  cout << myname << "  fitusestuck: " << fitusestuck << endl;
  cout << myname << "  ADC fit range: (" << iadcfitmin << ", " << iadcfitmax << ")" << endl;
  cout << myname << "  Vin fit range: (" << vinfitmin << ", " << vinfitmax << ")" << endl;
  pfit = phf->GetFunction("pol1");
  fitped = pfit->GetParameter(0);
  fitVinPerAdc = pfit->GetParameter(1);
  calib.gain = fitVinPerAdc;
  calib.offset = fitped;
  cout << myname << "Fit gain: " << fitVinPerAdc << " mV/ADC, pedestal: " << fitped << " mV" << endl;
  ostringstream ssdif;
  ssdif.precision(3);
  ssdif << "V_{in} - (" << fitVinPerAdc << " ADC + " << fitped << ") [mV]";
  phd->GetYaxis()->SetTitle(ssdif.str().c_str());
  phdw->GetYaxis()->SetTitle(ssdif.str().c_str());
  // Find the nominal calibration.
  // If datasetCalib is set, use its calibration for this channel.
  // Otherwise, use the gain from the reader and the pedestal from data at ADC = 500.
  bool useNomGain = datasetCalib.size() == 0;
  if ( ! useNomGain ) {
    cout << myname << "Taking nominal calibration from dataset " << datasetCalib << endl;
    pcalNominal = AdcChannelCalibration::find(datasetCalib, reader.chip(), reader.channel());
    if ( pcalNominal == nullptr ) {
      cout << myname << "Nominal calibration not found!" << endl;
      useNomGain = true;
    } else {
      ostringstream ssdifn;
      ssdifn.precision(3);
      ssdifn << "V_{in} - V_{in}(" << datasetCalib << ") [mV]";
      phn->GetYaxis()->SetTitle(ssdifn.str().c_str());
      phnw->GetYaxis()->SetTitle(ssdifn.str().c_str());
    }
  }
  if ( useNomGain ) {
    Index adc_nom = 500;
    cout << myname << "Nominal gain taken from reader, offset from data at ADC = " << adc_nom << endl;
    nomped = fitped + (fitVinPerAdc - nomVinPerAdc)*adc_nom;
    cout << myname << "Nominal gain: " << nomVinPerAdc << " mV/ADC, pedestal: " << nomped << " mV" << endl;
    ostringstream ssdifn;
    ssdifn.precision(3);
    ssdifn << "V_{in} - (" << nomVinPerAdc << " ADC + " << nomped << ") [mV]";
    phn->GetYaxis()->SetTitle(ssdifn.str().c_str());
    phnw->GetYaxis()->SetTitle(ssdifn.str().c_str());
  }
  // Fill the remaining histograms.
  for ( Index iadc=0; iadc<nadc; ++iadc ) {
    for ( Index ivin=0; ivin<nvin; ++ivin ) {
      double vin = reader.vin(ivin);
      double evinLinear = fitped + iadc*fitVinPerAdc;
      double evinCalib  = vinCalib(iadc);
      Index count = reader.countTable()[iadc][ivin];
      phd->Fill( iadc, vin - evinLinear, count);
      phdw->Fill(iadc, vin - evinLinear, count);
      phn->Fill( iadc, vin - evinCalib,  count);
      phnw->Fill(iadc, vin - evinCalib,  count);
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
    double xmLinear = fitVinPerAdc*iadc + fitped;
    calib.calCounts[iadc] = count;
    calib.calMeans[iadc] = xm + xmLinear;
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
      bool bad = !fitusestuck && isStuck;
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
  string shnam = sshnam.str();
  Index bin = iadc + 1;
  TH1* ph = phc->ProjectionY(shnam.c_str(), bin, bin);
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

double AdcSampleAnalyzer::calMean(Index iadc) const {
  if ( phm == nullptr ) return 0.0;
  if ( int(iadc) >= phm->GetNbinsX() ) return 0.0;
  double mean0 = fitVinPerAdc*iadc + fitped;
  double dmean = phm->GetBinContent(iadc+1);
  return mean0 + dmean;
}

//**********************************************************************

double AdcSampleAnalyzer::calRms(Index iadc) const {
  if ( phs == nullptr ) return 0.0;
  if ( int(iadc) >= phs->GetNbinsX() ) return 0.0;
  return phs->GetBinContent(iadc+1);
}

//**********************************************************************

double AdcSampleAnalyzer::vinCalib(Index iadc) const {
  if ( pcalNominal != nullptr ) {
    return pcalNominal->calMean(iadc);
  }
  return nomped + iadc*nomVinPerAdc;
}

//**********************************************************************

AdcSampleAnalyzer::AdcVoltageResponseVector&
AdcSampleAnalyzer::evaluateVoltageResponses(double vmin, double vmax, Index nv) {
  const string myname = "AdcSampleAnalyzer::evaluateVoltageResponses: ";
  if ( nv == 0 ) return voltageResponses;
  if ( vmin >= vmax ) return voltageResponses;
  voltageResponses.resize(0);
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
    AdcVoltageResponse& avr = voltageResponses.back();
    TH2* ph = phc;     // Input voltage vs ADC bin
    int nadc = ph->GetNbinsX();
    int nhv = ph->GetNbinsY();
    TAxis* paxv = ph->GetYaxis();
    int ihv1 = paxv->FindFixBin(v1);
    int ihv2 = paxv->FindFixBin(v2);
    //cout << myname << "Processing voltage bin " << iv << endl;
    //cout << myname << "  V bins: " << ihv1 << ", " << ihv2 << endl;
    //cout << myname << "   N ADC: " << nadc << endl;
    double sum = 0;
    for ( int ihv=ihv1; ihv<ihv2; ++ihv ) {
      for ( int iadc=0; iadc<nadc; ++iadc ) {
        int iha = iadc + 1;
        int ibin = ph->GetBin(iha, ihv);
        double nsam = ph->GetBinContent(ibin);
        if ( nsam > 0 ) avr.addSample(iadc, nsam);
        sum += nsam;
      }
    }
    avr.close();
    //cout << myname << "  # ADC bins for " << iv << ": " << avr.fractions.size()
    //     << " (" << sum << ")"
    //     << endl;
  }
  return voltageResponses;
}

//**********************************************************************

const FloatVector&
AdcSampleAnalyzer::evaluateVoltageEfficiencies(double rmsmax) {
  static FloatVector empty;
  const string myname = "AdcSampleAnalyzer::evaluateVoltageEfficiencies: ";
  unsigned int nvr = voltageResponses.size();
  if ( phc ==  nullptr ) return empty;
  if ( nvr < 1 ) return empty;
  // Create voltage performance object.
  double v1 = voltageResponses[0].vmin;
  double v2 = voltageResponses[nvr-1].vmax;
  vperfs.emplace_back(chip(), channel(), rmsmax, nvr, v1, v2);
  AdcVoltagePerformance& vperf = vperfs.back();
  // Create efficiency hisogram.
  ostringstream sshnam;
  sshnam << "hveff" << rmsmax;
  string shnam = sshnam.str();
  ostringstream sstitl;
  sstitl << phc->GetTitle();
  sstitl << " efficiency for RMS < " << rmsmax << " mV";
  sstitl << ";V_{in} [mV]";
  sstitl << ";Efficiency";
  string stitl = sstitl.str();
  for ( char& ch : shnam ) if ( ch == '.' ) ch = 'p';
  phveff = new TH1F(shnam.c_str(), stitl.c_str(), nvr, v1, v2);
  phveff->SetStats(0);
  phveff->SetMinimum(0.0);
  phveff->SetMaximum(1.03);
  phveff->SetLineWidth(2);
  // Create mean RMS histogram.
  sshnam.str("");
  sshnam << "hvrms" << rmsmax;
  shnam = sshnam.str();
  sstitl.str("");
  sstitl << phc->GetTitle();
  sstitl << " mean RMS for RMS < " << rmsmax << " mV";
  sstitl << ";V_{in} [mV]";
  sstitl << ";RMS(V_{in}) [mV]";
  stitl = sstitl.str();
  phvrms = new TH1F(shnam.c_str(), stitl.c_str(), nvr, v1, v2);
  phvrms->SetStats(0);
  phvrms->SetMinimum(0.0);
  phvrms->SetMaximum(1.03);
  phvrms->SetLineWidth(2);
  // Fill voltage performance and histograms.
  double dv = (v2 - v1)/nvr;
  double dvhalf = 0.5*dv;
  FloatVector gx(nvr, 0.0);
  FloatVector gy(nvr, 0.0);
  FloatVector gexlo(nvr, dvhalf);
  FloatVector gexhi(nvr, dvhalf);
  FloatVector geylo(nvr, 0.0);
  FloatVector geyhi(nvr, 0.0);
  for ( unsigned int ivr=0; ivr<nvr; ++ivr ) {
    AdcVoltageResponse& avr = voltageResponses[ivr];
    Index iadc1 = avr.bin0;
    Index iadc2 = iadc1 + avr.fractions.size();
    double eff = 0.0;
    double count = 0.0;
    double rmsSum = 0.0;
    double rms2Sum = 0.0;
    double losum = 0.0;
    double hisum = 0.0;
    double drmslo2Sum = 0.0;
    double drmshi2Sum = 0.0;
    double rmsMean = 0.0;
    // Loop twice finding the mean on the first pass.
    for ( int haveMean=0; haveMean<2; ++haveMean ) {
      for ( unsigned int iadc=iadc1; iadc<iadc2; ++iadc ) {
        count += avr.count(iadc);
        double rms = calRms(iadc);
        if ( rms >= 0.0 && rms < rmsmax ) {      // RMS < 0 is uncalibrated bin treated here as bad
          double frac = avr.fraction(iadc);
          if ( ! haveMean ) {
            eff += frac;
            rmsSum += frac*rms;
            rms2Sum += frac*rms*rms;
          } else {
            double drms = rms - rmsMean;
            double drms2 = drms*drms;
            if ( drms < 0.0 ) {
              losum += frac;
              drmslo2Sum += frac*drms2;
            } else if ( drms > 0.0 ) {
              hisum += frac;
              drmshi2Sum += frac*drms2;
            } else {
              double wt = 0.5*frac;
              losum += wt;
              hisum += wt;
              drmslo2Sum += wt*drms2;
              drmshi2Sum += wt*drms2;
            }
          }
        }
      }
      if ( ! haveMean ) rmsMean = eff > 0.0 ? rmsSum/eff : 0.0;
    }
    double rms2Mean = eff > 0.0 ? rms2Sum/eff : 0.0;
    double diff = rms2Mean - rmsMean*rmsMean;
    if ( diff < 0.0 ) diff = 0.0;
    double rmsRms = sqrt(diff);
    double rmsRmslo = losum > 0 ? sqrt(drmslo2Sum/losum) : 0.0;
    double rmsRmshi = hisum > 0 ? sqrt(drmshi2Sum/hisum) : 0.0;
    vperf.vinCounts[ivr] = count;
    vperf.vinEffs[ivr] = eff;
    vperf.vinResMeans[ivr] = rmsMean;
    vperf.vinResRmss[ivr] = rmsRms;
    vperf.vinResRmslos[ivr] = rmsRmslo;
    vperf.vinResRmshis[ivr] = rmsRmshi;
    double deff = vperf.deff(ivr);
    phveff->SetBinContent(ivr+1, eff);
    phveff->SetBinError(ivr+1, deff);
    phvrms->SetBinContent(ivr+1, rmsMean);
    phvrms->SetBinError(ivr+1, rmsRms);
    gx[ivr] = v1 + dvhalf + ivr*dv;
    gy[ivr] = rmsMean;
    geylo[ivr] = rmsRmslo;
    geyhi[ivr] = rmsRmshi;
  }
  pgvrms = new TGraphAsymmErrors(nvr, &gx[0], &gy[0], &gexlo[0], &gexhi[0], &geylo[0], &geyhi[0]);
  pgvrms->SetLineWidth(2);
  return vperf.vinEffs;
}

//**********************************************************************
