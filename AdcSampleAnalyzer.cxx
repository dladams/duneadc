// AdcSampleAnalyzer.cxx

#include "AdcSampleAnalyzer.h"
#include "AdcTreeChannelCalibration.h"
#include "AdcPedestalChannelCalibration.h"
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include "TH2F.h"
#include "TH1Props.h"
#include "TCanvas.h"
#include "TLatex.h"
#include "TLegend.h"
#include "TLine.h"
#include "TDirectory.h"
#include "TSystem.h"
#include "TROOT.h"

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;
using std::find;

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

AdcSampleAnalyzer::
AdcSampleAnalyzer(const AdcSampleReader& rdr, const AdcChannelCalibration* pcal, bool fixped)
: pcalNominal(pcal),
  pfit(nullptr), fitGain(0.0), fitOffset(0.0),
  m_preader(&rdr),
  m_dataset(rdr.dataset()),
  m_sampleName(rdr.sample()),
  m_refCalib(m_localTreeCalib),
  m_chip(rdr.chip()),
  m_channel(rdr.channel()),
  m_time(rdr.time()),
  m_nadc(rdr.nadc()) {
  const string myname = "AdcSampleAnalyzer::ctor: ";
  if ( rdr.countTable().size() == 0 ) {
    cout << myname << "ERROR: Reader table is empty." << endl;
    return;
  }
  iadcfitmin = nadc()==4096 ? 128 : 1;
  iadcfitmax = nadc() - 1;
  vinfitmin = 0.0;
  vinfitmax = 1600.0;
  fitusestuck = false;
  Index adcmax = nadc();
  Index nvin = rdr.nvin();
  adcUnderflow = 64;  // This and below are considered underflow
  if ( sampleName().find("ltc") != string::npos ) {
    fitusestuck = true;
    if ( nominalGain == 0.0 ) nominalGain = 0.1151;
    adcUnderflow = 0;
  }
  cout << myname << "Processing sample " << rdr.sample();
  cout << ": chip " << chip() << ", channel " << channel()
       << ", time " << time()
       << " (" << TDatime(time()).AsString() << ")"
       << endl;
  nsample = rdr.nsample();
  localCalib().data().chip = chip();
  localCalib().data().chan = channel();
  localCalib().data().time = time();
  // Create the histograms.
  createHistograms(nvin, rdr.vinmin(), rdr.vinmax());
  double countPerVinBin = double(nsample)/nadc();
  minCountForStats = 2 + countPerVinBin/1000;
  cout << myname << "# ADC: " << nadc() << endl;
  cout << myname << "# Vin: " << nvin << endl;
  cout << myname << "Min count for stats: " << minCountForStats << endl;
  // Create the fit and response histograms.
  Index nfill = 0;
  Index nskip = 0;
  for ( Index iadc=0; iadc<nadc(); ++iadc ) {
    for ( Index ivin=0; ivin<nvin; ++ivin ) {
      double vin = rdr.vinCenter(ivin);
      Index count = rdr.countTable()[iadc][ivin];
      phc->Fill(iadc, vin, count);
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
  bool isBatch = gROOT->IsBatch();
  if ( ! isBatch ) gROOT->SetBatch(true);
  TCanvas* pcantmp = new TCanvas;
  if ( ! isBatch ) gROOT->SetBatch(false);
  phf->Fit("pol1", "Q");
  delete pcantmp;
  cout << myname << "  fitusestuck: " << fitusestuck << endl;
  cout << myname << "  ADC fit range: (" << iadcfitmin << ", " << iadcfitmax << ")" << endl;
  cout << myname << "  Vin fit range: (" << vinfitmin << ", " << vinfitmax << ")" << endl;
  pfit = phf->GetFunction("pol1");
  fitOffset = pfit->GetParameter(0);
  fitGain = pfit->GetParameter(1);
  localCalib().data().gain = fitGain;
  localCalib().data().offset = fitOffset;
  cout << myname << "Fit gain: " << fitGain << " mV/ADC, offset: " << fitOffset << " mV" << endl;
  ostringstream ssdif;
  ssdif.precision(3);
  ssdif << "V_{in} - (" << fitGain << " ADC + " << fitOffset << ") [mV]";
  phd->GetYaxis()->SetTitle(ssdif.str().c_str());
  phdw->GetYaxis()->SetTitle(ssdif.str().c_str());
  // Fill the linear response difference histograms.
  for ( Index iadc=0; iadc<nadc(); ++iadc ) {
    for ( Index ivin=0; ivin<nvin; ++ivin ) {
      double vin = rdr.vinCenter(ivin);
      double evinLinear = fitOffset + iadc*fitGain;
      Index count = rdr.countTable()[iadc][ivin];
      phd->Fill( iadc, vin - evinLinear, count);
      phdw->Fill(iadc, vin - evinLinear, count);
    }
  }
  // Fill diff stat histograms.
  localCalib().data().calMeans.resize(nadc());
  localCalib().data().calRmss.resize(nadc());
  localCalib().data().calCounts.resize(nadc());
  localCalib().data().calTails.resize(nadc());
  cout << myname << "Tail fraction uses pull: " << tailFracUsesPull << endl;
  if ( tailFracUsesPull ) {
    cout << myname << "Tail is pull > " <<  pullthresh << endl;
  } else {
    cout << myname << "Tail is deviation > " << tailWindow << " mV" << endl;
  }
  for ( Index iadc=0; iadc<nadc(); ++iadc ) {
    TH1* ph = hdiffcalib(iadc);
    if ( ph == nullptr ) continue;
    double xm = -99999.;
    double xs = -99.;
    double xsx = -99.;
    unsigned int count = ph->Integral();
    double tailfrac = 0.0;
    if ( count >= minCountForStats ) {
      xm = ph->GetMean();
      xs = ph->GetRMS();
      TH1Props hp(ph);
      xsx = hp.expandedRMS(pullthresh);
      if ( tailFracUsesPull ) tailfrac = hp.tailFrac(pullthresh);
      else tailfrac = hp.fracOutsideMean(tailWindow);
    }
    double xmLinear = fitGain*iadc + fitOffset;
    localCalib().data().calCounts[iadc] = count;
    localCalib().data().calMeans[iadc] = xm + xmLinear;
    localCalib().data().calRmss[iadc] = xs;
    localCalib().data().calTails[iadc] = tailfrac;
    bool isStuck = sticky(iadc);
    double xsg = isStuck ? 0 : xs;
    double xsb = !isStuck ? 0 : xs;
    double xr = sqrt(xm*xm+xs*xs);
    phm->SetBinContent(iadc+1, xm);
    phs->SetBinContent(iadc+1, xs);
    pht->SetBinContent(iadc+1, tailfrac);
    phsx->SetBinContent(iadc+1, xsx);
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
    if ( iadc > 64 && !isStuck ) {
      phdn->Fill(xr);
    }
  }
  // Fill the calibration difference histograms.
  // Find the nominal calibration.
  if ( pcalNominal != nullptr ) {
    ostringstream ssdifn;
    ssdifn.precision(3);
    ssdifn << "V_{in} - V_{in}(" << pcalNominal->name() << ") [mV]";
    phn->GetYaxis()->SetTitle(ssdifn.str().c_str());
    phnw->GetYaxis()->SetTitle(ssdifn.str().c_str());
    phvn->GetYaxis()->SetTitle(ssdifn.str().c_str());
    cout << myname << "Calibration is for chip " << pcalNominal->chip()
         << ", channel " << pcalNominal->channel()
         << ", time " << pcalNominal->time()
         << " (" << TDatime(pcalNominal->time()).AsString() << ")"
         << endl;
    if ( fixped ) {
      cout << myname << "Applying pedestal correction to calibration." << endl;
      pcalNominal =
        new AdcPedestalChannelCalibration(*pcalNominal, 290, 311, localCalib(), 1.0);
    }
    for ( Index iadc=0; iadc<nadc(); ++iadc ) {
      double evinCalib = pcalNominal == nullptr ? 0.0 : nominalCalibrationVin(iadc);
      double ermsCalib = pcalNominal == nullptr ? 0.0 : nominalCalibrationRms(iadc);
      for ( Index ivin=0; ivin<nvin; ++ivin ) {
        double vin = rdr.vinCenter(ivin);
        Index count = rdr.countTable()[iadc][ivin];
        phn->Fill( iadc, vin - evinCalib,  count);
        phnw->Fill(iadc, vin - evinCalib,  count);
        if ( ermsCalib < 1.0 ) phvn->Fill(vin, vin - evinCalib,  count);
      }
    }
  } else {
    cout << myname << "Nominal calibration is not provided." << endl;
  }
  // Report memory at end of processing.
  if ( true ) {
    ProcInfo_t info;
    gSystem->GetProcInfo(&info);
    cout << myname << "Virtual memory at end of ctor is " << info.fMemVirtual/1000000.0 << " GB" << endl;
  }
  return;
}

//**********************************************************************

AdcSampleAnalyzer::
AdcSampleAnalyzer(AdcSampleReaderPtr preader, const AdcChannelCalibration* pcal, bool fixped)
: AdcSampleAnalyzer(*preader, pcal, fixped) {
  m_preaderManaged.swap(preader);
}

//**********************************************************************

AdcSampleAnalyzer::
AdcSampleAnalyzer(const AdcChannelCalibration& a_calib, Name a_sampleName, Name a_dataset)
: pcalNominal(nullptr),
  m_dataset(a_dataset), m_sampleName(a_sampleName), m_refCalib(a_calib),
  m_chip(calib().chip()),
  m_channel(calib().channel()),
  m_time(calib().time()),
  m_nadc(calib().size()) {
  const string myname = "AdcSampleAnalyzer::ctor: ";
  if ( m_dataset.size() == 0 ) {
    m_dataset = m_sampleName.substr(0, m_sampleName.find("_"));
  }
  cout << myname << "Processing calibration " << calib().name()
       << " assigned sample name " << sampleName()
       << " and dataset name " << dataset() << endl;
  fitGain = calib().linearGain();
  fitOffset = calib().linearOffset();
  adcUnderflow = 64;
  createHistograms(2000, 0, 2000);
  for ( Index iadc=0; iadc<nadc(); ++iadc ) {
    double fitMean = fitGain*iadc + fitOffset;
    double calMean = calib().calMean(iadc);
    double xm = calMean - fitMean;
    double xs = calib().calRms(iadc);
    bool isStuck = sticky(iadc);
    double xsg = isStuck ? 0 : xs;
    double xsb = !isStuck ? 0 : xs;
    double xr = sqrt(xm*xm+xs*xs);
    phm->SetBinContent(iadc+1, xm);
    phs->SetBinContent(iadc+1, xs);
    phsg->SetBinContent(iadc+1, xsg);
    phsb->SetBinContent(iadc+1, xsb);
    phr->SetBinContent(iadc+1, xr);
    pht->SetBinContent(iadc+1, calib().calTail(iadc));
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
  }
}

//**********************************************************************

AdcSampleAnalyzer::~AdcSampleAnalyzer() {
  clean();
  // Delete the long-term histograms.
  for ( TH1* ph : m_saveHists ) delete ph;
  m_saveHists.clear();
  // Clear the bars. Saves a little space but spoils on-screen performance plots.
  bool rembars = gROOT->IsBatch();
  if ( rembars ) {
    for ( TLine* pline : g80bars ) delete pline;
    g80bars.clear();
    for ( TLine* pline : g100bars ) delete pline;
    g100bars.clear();
  }
  if ( manageCalNominal ) {
    delete pcalNominal;
    pcalNominal = nullptr;
  }
}

//**********************************************************************

void AdcSampleAnalyzer::manageHist(TH1* ph, bool cleaned) const {
  const Name myname = "AdcSampleAnalyzer::manageHist: ";
  bool doCheck = true;
  if ( doCheck ) {
    if ( find(m_saveHists.begin(), m_saveHists.end(), ph) != m_saveHists.end() ||
         find(m_cleanHists.begin(), m_cleanHists.end(), ph) != m_cleanHists.end() ) {
      cout << "WARNING: Ignoring second attempt to manage a histogram." << endl;
      return;
    }
  }
  if ( cleaned ) m_cleanHists.push_back(ph);
  else m_saveHists.push_back(ph);
}

//**********************************************************************

void AdcSampleAnalyzer::clean() {
  // Delete the histograms managed locally.
  for ( TH1* ph : m_cleanHists ) delete ph;
  m_cleanHists.clear();
  // Delete the analyzer if it is managed here.
  if ( m_preaderManaged ) {
    m_preaderManaged.reset(nullptr);
    m_preader = nullptr;
  }
}

//**********************************************************************

TH1* AdcSampleAnalyzer::hcalib(Index iadc) const {
  if ( phc == nullptr ) return nullptr;
  ostringstream sshnam;
  sshnam << phc->GetName() << "_adc" << iadc;
  string shnam = sshnam.str();
  Index bin = iadc + 1;
  TH1* ph = phc->ProjectionY(shnam.c_str(), bin, bin);
  ph->SetDirectory(nullptr);
  cleanHist(ph);
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
  ph->SetDirectory(nullptr);
  cleanHist(ph);
  ostringstream sshtitl;
  sshtitl << phd->GetTitle() << " bin " << iadc;
  string shtitl = sshtitl.str();
  ph->SetTitle(shtitl.c_str());
  ph->SetLineWidth(2);
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
  ph->SetDirectory(nullptr);
  cleanHist(ph);
  ostringstream sshtitl;
  sshtitl << phn->GetTitle() << " bin " << iadc;
  string shtitl = sshtitl.str();
  ph->SetTitle(shtitl.c_str());
  ph->SetLineWidth(2);
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
    ph->SetDirectory(nullptr);
    cleanHist(ph);
    ostringstream sshtitl;
    sshtitl << phdw->GetTitle() << " bin " << iadc;
    string shtitl = sshtitl.str();
    ph->SetTitle(shtitl.c_str());
    ph->SetLineWidth(2);
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
  double mean0 = fitGain*iadc + fitOffset;
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

double AdcSampleAnalyzer::calExpandedRms(Index iadc) const {
  if ( phsx == nullptr ) return 0.0;
  if ( int(iadc) >= phsx->GetNbinsX() ) return 0.0;
  return phsx->GetBinContent(iadc+1);
}

//**********************************************************************

double AdcSampleAnalyzer::calTail(Index iadc) const {
  if ( pht == nullptr ) return 0.0;
  if ( int(iadc) >= pht->GetNbinsX() ) return 0.0;
  return pht->GetBinContent(iadc+1);
}

//**********************************************************************

double AdcSampleAnalyzer::nominalCalibrationVin(Index iadc) const {
  const string myname = "AdcSampleAnalyzer::vinCalib: ";
  if ( pcalNominal != nullptr ) {
    return pcalNominal->calMean(iadc);
  }
  cout << myname << "ERROR: Sample analyzer does not have a nominal calibration." << endl;
  return 0.0;
}

//**********************************************************************

double AdcSampleAnalyzer::nominalCalibrationRms(Index iadc) const {
  const string myname = "AdcSampleAnalyzer::nominalCalibrationRms: ";
  if ( pcalNominal != nullptr ) {
    return pcalNominal->calRms(iadc);
  }
  cout << myname << "ERROR: Sample analyzer does not have a nominal calibration." << endl;
  return 0.0;
}

//**********************************************************************

double AdcSampleAnalyzer::nominalCalibrationTail(Index iadc) const {
  const string myname = "AdcSampleAnalyzer::nominalCalibrationTail: ";
  if ( pcalNominal != nullptr ) {
    return pcalNominal->calTail(iadc);
  }
  cout << myname << "ERROR: Sample analyzer does not have a nominal calibration." << endl;
  return 0.0;
}

//**********************************************************************

AdcSampleAnalyzer::AdcVoltageResponseVector&
AdcSampleAnalyzer::evaluateVoltageResponses(double vmin, double vmax, Index nv) {
  const string myname = "AdcSampleAnalyzer::evaluateVoltageResponses: ";
  if ( nv == 0 ) return voltageResponses;
  if ( vmin >= vmax ) return voltageResponses;
  TH2* ph = phc;     // Input voltage vs ADC bin
  voltageResponses.resize(0);
  if ( ph->GetEntries() == 0 ) {
    cout << myname << "Sample analyzer has no data (phc is empty)." << endl;
    return voltageResponses;
  }
  float dv = (vmax - vmin)/nv;
  float v1 = vmin;
  float v2 = v1 + dv;
  AdcChannelId id(chip(), channel());
  for ( Index iv=0; iv<nv; ++iv ) {
    if ( iv > 0 ) {
      v1 = v2;
      v2 += dv;
    }
    voltageResponses.push_back(AdcVoltageResponse(id, v1, v2));
    AdcVoltageResponse& avr = voltageResponses.back();
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
AdcSampleAnalyzer::evaluateVoltageEfficiencies(double rmsmax, bool readData, bool dropTails) {
  const string myname = "AdcSampleAnalyzer::evaluateVoltageEfficiencies: ";
  static FloatVector empty;
  evaluateReadData = readData;
  unsigned int nvr = voltageResponses.size();
  ostringstream sschan;
  sschan << channel();
  Name schan = sschan.str();
  string titlePrefix = sampleName() + " channel " + schan;
  if ( nvr < 1 ) {
    cout << myname << "Voltage responses not found. Please call:" << endl;
    cout << myname << "  evaluateVoltageReponses(vmin, vmax, nv)" << endl;
    return empty;
  }
  // Create voltage performance object.
  double v1 = voltageResponses[0].vmin;
  double v2 = voltageResponses[nvr-1].vmax;
  vperfs.emplace_back(chip(), channel(), time(), rmsmax, nvr, v1, v2);
  AdcVoltagePerformance& vperf = vperfs.back();
  string hnambase = "h" + sampleName() + "_" + schan + "_";
  // Create efficiency histogram.
  ostringstream sshnam;
  sshnam << hnambase << "hveff" << rmsmax;
  string shnam = sshnam.str();
  ostringstream sstitl;
  sstitl << titlePrefix;
  sstitl << " efficiency for RMS < " << rmsmax << " mV";
  if ( dropTails ) sstitl << ", notail";
  if ( pcalNominal != nullptr ) sstitl << ", calib " << pcalNominal->name();
  sstitl << ";V_{in} [mV]";
  sstitl << ";Efficiency";
  string stitl = sstitl.str();
  for ( char& ch : shnam ) if ( ch == '.' ) ch = 'p';
  phveff = createManagedHistogram(shnam, stitl, nvr, v1, v2);
  phveff->SetDirectory(nullptr);
  phveff->SetStats(0);
  phveff->SetMinimum(0.0);
  phveff->SetMaximum(1.03);
  phveff->SetLineWidth(2);
  // Create mean RMS histogram.
  if ( readData ) {
    sshnam.str("");
    sshnam << hnambase << "hvrms" << rmsmax;
    shnam = sshnam.str();
    sstitl.str("");
    sstitl << titlePrefix;
    sstitl << " mean RMS for RMS < " << rmsmax << " mV";
    sstitl << ";V_{in} [mV]";
    sstitl << ";RMS(V_{in}) [mV]";
    stitl = sstitl.str();
    phvrms = createManagedHistogram(shnam, stitl, nvr, v1, v2);
    phvrms->SetStats(0);
    phvrms->SetMinimum(0.0);
    phvrms->SetMaximum(rmsmax);
    phvrms->SetLineWidth(2);
  }
  // Create tail histogram.
  sshnam.str("");
  sshnam << hnambase  << "hvtail" << rmsmax;
  shnam = sshnam.str();
  sstitl.str("");
  sstitl << titlePrefix;
  sstitl << " tail fraction for RMS < " << rmsmax << " mV";
  sstitl << ";V_{in} [mV]";
  sstitl << ";Tail fraction";
  stitl = sstitl.str();
  for ( char& ch : shnam ) if ( ch == '.' ) ch = 'p';
  phvtail = createManagedHistogram(shnam, stitl, nvr, v1, v2);
  phvtail->SetStats(0);
  phvtail->SetMinimum(1.e-6);
  phvtail->SetMaximum(1.0);
  phvtail->SetLineWidth(2);
  // Fill voltage performance and histograms.
  double dv = (v2 - v1)/nvr;
  double dvhalf = 0.5*dv;
  FloatVector gx(nvr, 0.0);
  FloatVector gexlo(nvr, dvhalf);
  FloatVector gexhi(nvr, dvhalf);
  FloatVector geylo(nvr, 0.0);
  FloatVector geyhi(nvr, 0.0);
  FloatVector gey0(nvr, 0.0);
  FloatVector gey10(nvr, 0.0);
  FloatVector gey90(nvr, 0.0);
  FloatVector gey100(nvr, 0.0);
  // Loop over performance voltage bins.
  bool remhist = true;
  if ( ! remhist ) cout << myname << "WARNING: Histograms are not being removed." << endl;
  for ( unsigned int ivr=0; ivr<nvr; ++ivr ) {
    AdcVoltageResponse& avr = voltageResponses[ivr];
    double vin1 = avr.vmin;
    double vin2 = avr.vmax;
    Index iadc1 = avr.bin0;
    Index iadc2 = iadc1 + avr.fractions.size();
    double countSum = 0.0;
    double rmsDev = 0.0;    // RMS(Vmeas - Vtrue)
    double tailFrac = 0.0;
    // Histogram used to get RMS and RMS extent
    sshnam.str("");
    sshnam << "hverr_" << ivr;
    shnam = sshnam.str();
    TH1* phverr = new TH1F(shnam.c_str(), "", 1000, 0, rmsmax);
    if ( remhist ) {
      phverr->SetDirectory(nullptr);
    }
    // Evaluate efficiency, resolution and tail fraction using the input data.
    double effSum = 0.0;
    if ( readData ) {
      if ( reader() == nullptr ) {
        cout << myname << "ERROR: Reader has been removed." << endl;
        return empty;
      }
      const AdcSampleReader& rdr = *reader();
      Index ictvin = 0;                 // Voltage index for the reader count table
      Index nctvin = rdr.nvin();     // Maximum voltage index for the reader count table
      double devSum = 0.0;
      double dev2Sum = 0.0;
      ostringstream sshnam;
      sshnam << "hverr_" << ivr;
      string shnam = sshnam.str();
      double tailFracSum = 0.0;
      // Skip reader voltage bins below the current range.
      while ( rdr.vinCenter(ictvin) < vin1 && ictvin < nctvin ) ++ictvin;
      // Loop over reader voltage bins in the current range.
      while ( rdr.vinCenter(ictvin) < vin2 && ictvin < nctvin ) {
        double vinTrue = rdr.vinCenter(ictvin);
        if ( vinTrue >= vin2 ) break;    // We have reached the next voltage bin.
        // Loop over reader ADC bins.
        for ( Index iadc=0; iadc<rdr.countTable().size(); ++iadc ) {
          Index count = rdr.countTable()[iadc][ictvin];
          if ( count == 0 ) continue;
          countSum += count;
          double vinMeasured = calMean(iadc);
          double rmsMeasured = calRms(iadc);
          double tailFrac = calTail(iadc);
          if ( pcalNominal != nullptr ) {
            vinMeasured = nominalCalibrationVin(iadc);
            rmsMeasured = nominalCalibrationRms(iadc);
            tailFrac = nominalCalibrationTail(iadc);
          }
          double dvin = vinMeasured - vinTrue;
          double dvin2 = dvin*dvin;
          double apull = fabs(dvin/rmsMeasured);
          double advin = fabs(dvin);
          bool skipBin = false;
          skipBin |= rmsMeasured < 0.0;             // Bin has too few measurements
          skipBin |= rmsMeasured >= rmsmax;         // Bin RMS is too large
          skipBin |= dropTails && tailFrac > 0.0;   // Drop bins with tails
          if ( ! skipBin ) {      // RMS < 0 is uncalibrated bin treated here as bad
            phverr->Fill(rmsMeasured, count);
            effSum += count;
            devSum += count*dvin;
            dev2Sum += count*dvin2;
            if ( tailFracUsesPull && apull > pullthresh ) tailFracSum += count;
            if ( !tailFracUsesPull && advin > tailWindow ) tailFracSum += count;
          }
        }
        ++ictvin;
      }
      if ( effSum > 0.0 ) {
        double devMean = devSum/effSum;
        double dev2Mean = dev2Sum/effSum;
        rmsDev = sqrt(dev2Mean);        // RMS deviation
        tailFrac = tailFracSum/effSum;
      }
      phvrms->SetBinContent(ivr+1, rmsDev);
    // Evaluate efficiency, resolution and tail fraction using the calibration.
    } else {
      // Loop over ADC bins in this voltage bin.
      for ( unsigned int iadc=iadc1; iadc<iadc2; ++iadc ) {
        Index count = avr.count(iadc);
        countSum += count;
        double rms = calRms(iadc);
        if ( rms >= 0.0 && rms < rmsmax ) {  // RMS < 0 is uncalibrated bin treated here as bad
          double frac = avr.fraction(iadc);
          effSum += count;
          double tFrac = calTail(iadc);
          tailFrac += frac*tFrac;
          phverr->Fill(rms, count);
        }
      }
    }
    double eff = effSum/countSum;
    double rmsInt0 = 0.0;    // 0% point of the calibration RMS
    double rmsInt10 = 0.00;  // 10% point of the calibration RMS
    double rmsInt90 = 0.00;  // 90% point of the calibration RMS
    double rmsInt100 = 0.00; // 100% point of the calibration RMS
    if ( effSum > 0.0 ) {
      // Process verr histogram.
      double sum = 0.0;
      int lastbin = phverr->GetNbinsX()+1;
      for ( int bin=0; bin<=lastbin; ++bin ) {
        double count = phverr->GetBinContent(bin);
        sum += count;
        double x = phverr->GetXaxis()->GetBinCenter(bin);
        if ( bin == 0 ) x = 0.0;
        if ( bin == lastbin ) x = rmsmax;
        if ( sum == count )      rmsInt0 = x;
        if ( sum <= 0.1*effSum ) rmsInt10 = x;
        if ( sum <= 0.9*effSum ) rmsInt90 = x;
        if ( count > 0 )         rmsInt100 = x;
      }
    }
    if ( remhist ) {
      delete phverr;
    }
    vperf.vinCounts[ivr] = countSum;
    vperf.vinEffs[ivr] = eff;
    vperf.vinDevRmss[ivr] = rmsDev;
    double deff = vperf.deff(ivr);
    phveff->SetBinContent(ivr+1, eff);
    phveff->SetBinError(ivr+1, deff);
    phvtail->SetBinContent(ivr+1, tailFrac);
    gx[ivr] = v1 + dvhalf + ivr*dv;
    gey0[ivr] = rmsInt0;
    gey10[ivr] = rmsInt10;
    gey90[ivr] = rmsInt90;
    gey100[ivr] = rmsInt100;
    double dymin = 0.01;
    if ( gey90[ivr] - gey10[ivr] < dymin ) {
      gey90[ivr] += 0.5*dymin;
      gey10[ivr] -= 0.5*dymin;
    }
  }
  g80bars.clear();
  g100bars.clear();
  for ( unsigned int ivr=0; ivr<nvr; ++ivr ) {
    TLine* pline = new TLine(gx[ivr], gey10[ivr], gx[ivr], gey90[ivr]);
    pline->SetLineWidth(3);
    g80bars.push_back(pline);
    pline = new TLine(gx[ivr], gey0[ivr], gx[ivr], gey100[ivr]);
    pline->SetLineWidth(1);
    g100bars.push_back(pline);
  }
  return vperf.vinEffs;
}

//**********************************************************************

int AdcSampleAnalyzer::drawperf(bool dolabtail) const {
  const Name myname = "AdcSampleAnalyzer::drawperf: ";
  if ( phveff == nullptr ) {
    cout << myname << "Efficiency histogram not found. Please call" << endl;
    cout << myname << "  evaluateVoltageEfficiencies(rmsmax, readData, dropTails)" << endl;
    return 1;
  }
  if ( phvtail == nullptr ) {
    cout << myname << "ERROR: Tail histogram not found." << endl;
    return 2;
  }
  double ymax = 1.03;
  if ( phvrms != nullptr ) {
    double rmsmax = phvrms->GetMaximum();
    if ( rmsmax > ymax ) ymax = rmsmax;
  }
  TH1* ph = phveff;
  string hnam;
  // Build scaled tail plot.
  hnam = phvtail->GetName();
  hnam += "_scaled";
  TH1* phts = dynamic_cast<TH1*>(phvtail->Clone(hnam.c_str()));
  int tcol = 46;
  phts->SetLineColor(0);
  phts->SetFillColor(tcol);
  phts->SetDirectory(0);   // Leaking this preserves the line color/style in the legend
  for ( int bin=1; bin<=phts->GetNbinsX(); ++bin ) {
    double val = phvtail->GetBinContent(bin);
    double sval = val > 0.0 ? 0.6 + 0.1*log10(val) : 0.0;
    phts->SetBinContent(bin, sval);
    phts->SetBinError(bin, 0.0);
  }
  // Build labels for the tail plot.
  double x1 = phts->GetXaxis()->GetXmin();
  double x2 = phts->GetXaxis()->GetXmax();
  double xlab = x2 + 0.010*(x2-x1);
  double dylab = -0.005;
  vector<TLatex*> labs;
  labs.push_back(new TLatex(xlab, 0.0+dylab, "10^{-6}"));
  labs.push_back(new TLatex(xlab, 0.2+dylab, "10^{-4}"));
  labs.push_back(new TLatex(xlab, 0.4+dylab, "10^{-2}"));
  double xtitl = x2 + 0.09*(x2-x1);
  double ytitl = 0.1;
  TLatex* pttitl = new TLatex(xtitl, ytitl, "Tail fraction");
  pttitl->SetTextAngle(90);
  pttitl->SetTextFont(42);
  pttitl->SetTextSize(0.04);
  pttitl->SetTextColor(tcol);
  // Clone and draw efficiency.
  hnam = ph->GetName();
  hnam += "_vrms_vtail";
  TH1* phax = dynamic_cast<TH1*>(ph->Clone(hnam.c_str()));
  string htitl = phax->GetTitle();
  string::size_type ipos = htitl.find("efficiency");
  if ( ipos != string::npos ) {
    string newlab = evaluateReadData ? "actual" : "expected";
    newlab += " performance";
    htitl.replace(ipos, 10, newlab);
    phax->SetTitle(htitl.c_str());
  }
  string ylab = phax->GetYaxis()->GetTitle();
  ylab += ", V_{in} resolution [mV]";
  phax->GetYaxis()->SetTitle(ylab.c_str());
  phax->SetMaximum(ymax);
  phax->SetDirectory(0);   // Leaking this preserves the line color/style in the legend
  if ( gPad == nullptr ) new TCanvas;
  gPad->SetGridx();
  gPad->SetGridy();
  phax->DrawCopy("h");
  // Add labels for tail fraction.
  for ( TLatex* plab : labs ) {
    plab->SetTextSize(0.035);
    plab->SetTextFont(42);
    plab->SetTextColor(tcol);
    plab->Draw();
  }
  if ( dolabtail ) pttitl->Draw();
  // Add tail, eff again and rms.
  phts->DrawCopy("same");
  phax->DrawCopy("hist same");
  for ( TLine* pline : g100bars ) pline->Draw();
  for ( TLine* pline : g80bars ) pline->Draw();
  if ( phvrms != nullptr ) {
    phvrms->SetMarkerStyle(34);
    phvrms->DrawCopy("p same");
  }
  TLegend* pleg = new TLegend(0.3, 0.73, 0.55, 0.87);
  pleg->SetBorderSize(0);
  pleg->SetFillStyle(0);
  pleg->AddEntry(phax, "Efficiency", "l");
  pleg->AddEntry(g80bars[0], "Uncertainty (80%)", "l");
  if ( phvrms != nullptr ) {
    pleg->AddEntry(phvrms, "RMS deviation", "p");
  }
  pleg->AddEntry(phts, "Tail fraction", "f");
  pleg->Draw();
  TLine* pline = new TLine(x1, 0.0, x2, 0.0);
  pline->Draw();
  return 0;
}

//**********************************************************************

int AdcSampleAnalyzer::createHistograms(Index nvin, double vinmin, double vinmax) {
  const string myname = "AdcSampleAnalyzer::createHistograms: ";
  double zmax = 0;
  double dmax = 20;
  Index nd = 40;
  double wdmax = 10*dmax;
  double smax = 0.1*dmax;
  double gmin = 1.0;
  double gmax = 4.0;
  double lmin = 0.2;
  double lmax = 0.6;
  Index npadc = 4100*nadc()/4096;
  Index padcmax = npadc;
  int pvinmin = vinmin;
  int pvinmax = vinmax;
  Index npvin = pvinmax - pvinmin;
  cout << myname << "ADC binning: " << npadc << " (" << 0 << ", " << padcmax << ")" << endl;
  cout << myname << "Vin binning: " << npvin << " (" << pvinmin << ", " << pvinmax << ")" << endl;
  cout << myname << "  d binning: " << nd << " (" << 0 << ", " << dmax << ")" << endl;
  ostringstream sschan;
  sschan << channel();
  string schan = sschan.str();
  string stitle = sampleName() + " channel " + schan;
  string hnambase = "h" + sampleName() + "_" + schan + "_";
  string hnamf = hnambase + "fit";
  string hnamc = hnambase + "cor";
  string hnamd = hnambase + "dif";
  string hnamdw = hnambase + "difw";
  string hnamn = hnambase + "dfn";
  string hnamnw = hnambase + "dfnw";
  string hnamvn = hnambase + "vdfn";
  string hnamg = hnambase + "gai";
  string hnaml = hnambase + "lga";
  string hnamm = hnambase + "fmm";
  string hnams = hnambase + "fms";
  string hnamt = hnambase + "fmt";
  string hnamsx = hnambase + "fmsx";
  string hnamst = hnambase + "fmst";
  string hnamsg = hnambase + "fmsg";
  string hnamsb = hnambase + "fmsb";
  string hnamr = hnambase + "fmr";
  string hnamdn = hnambase + "fmdn";
  string hnamdr = hnambase + "fmdr";
  string hnamds = hnambase + "fmds";
  string hnamdsb = hnambase + "fmdsb";
  string stitlm = stitle + " fit mean";
  string stitlr = stitle + " fit RMS";
  string stitls = stitle + " fit sigma";
  string stitlt = stitle + " tail fraction";
  string stitlsx = stitle + " expanded fit sigma";
  string stitlst = stitle + " fit sigma tail";
  string stitldsb = stitle + " stuck";
  Index nvinperf = 80;
  double vinperfmin = 0.0;
  double vinperfmax = 1600.0;
  // Create histograms.
  phf = createManaged2dHistogram(hnamf, stitle, npadc, 0, padcmax, npvin, pvinmin, pvinmax);
  phc = createManaged2dHistogram(hnamc, stitle, npadc, 0, padcmax, npvin, pvinmin, pvinmax);
  phd = createManaged2dHistogram(hnamd, stitle, npadc, 0, padcmax, 400, -dmax, dmax);
  phdw = createManaged2dHistogram(hnamdw, stitle, npadc, 0, padcmax, 400, -wdmax, wdmax);
  phm = createManagedHistogram(hnamm, stitlm, npadc, 0, padcmax);
  phr = createManagedHistogram(hnamr, stitlr, npadc, 0, padcmax);
  phs = createManagedHistogram(hnams, stitls, npadc, 0, padcmax);
  pht = createManagedHistogram(hnamt, stitlt, npadc, 0, padcmax);
  phsx = createManagedHistogram(hnamsx, stitlsx, npadc, 0, padcmax);
  phdr = createManagedHistogram(hnamdr, stitle, nd, 0, dmax);
  phds = createManagedHistogram(hnamds, stitle, nd, 0, smax);
  phdsb = createManagedHistogram(hnamdsb, stitldsb, nd, 0, smax);
  vector<TH1*> hists2d = {phf, phc, phd};
  vector<TH1*> dhists = {phdr, phds, phdsb, phdw};
  // Add calibration histograms.
  if ( pcalNominal != nullptr ) {
    phn = createManaged2dHistogram(hnamn, stitle, npadc, 0, padcmax, 400, -dmax, dmax);
    phnw = createManaged2dHistogram(hnamnw, stitle, npadc, 0, padcmax, 400, -wdmax, wdmax);
    phvn = createManaged2dHistogram(hnamvn, stitle, nvinperf, vinperfmin, vinperfmax, 400, -dmax, dmax);
    phdn = createManagedHistogram(hnamdn, stitle, nd, 0, wdmax);
    phdn->GetXaxis()->SetTitle("Nominal resolution [mV]");
    hists2d.push_back(phn);
    hists2d.push_back(phnw);
    hists2d.push_back(phvn);
    dhists.push_back(phdn);
  }
  for ( TH1* ph : hists2d ) {
    ph->SetStats(0);
    ph->SetContour(20);
    if ( zmax > 0 ) ph->SetMaximum(zmax); 
    ph->GetXaxis()->SetTitle("ADC count");
    ph->SetLineWidth(2);
    ph->SetBit(TH1::kIsNotW);    // Turn off weights.
  }
  phc->GetYaxis()->SetTitleOffset(1.4);  // Make room for large labels on y-axis.
  for ( TH1* ph : dhists ) {
    //ph->SetStats(0);
    ph->GetYaxis()->SetTitle("# ADC bins");
    ph->SetLineWidth(2);
    ph->SetBit(TH1::kIsNotW);    // Turn off weights.
  }
  phdr->GetXaxis()->SetTitle("RMS [mV]");
  phds->GetXaxis()->SetTitle("Sigma [mV]");
  phdsb->GetXaxis()->SetTitle("Sigma [mV]");
  phc->GetYaxis()->SetTitle("Input level [mV]");
  phm->GetYaxis()->SetTitle("V_{in} diff mean [mV]");
  phr->GetYaxis()->SetTitle("V_{in} diff RMS [mV]");
  phs->GetYaxis()->SetTitle("V_{in} diff standard deviation [mV]");
  pht->GetYaxis()->SetTitle("Tail fraction");
  phsx->GetYaxis()->SetTitle("V_{in} diff expanded standard deviation [mV]");
  phm->SetStats(0);
  phd->SetStats(0);
  phdw->SetStats(0);
  phr->SetStats(0);
  phs->SetStats(0);
  pht->SetStats(0);
  phsx->SetStats(0);
  phm->SetMaximum(dmax);
  phm->SetMinimum(-dmax);
  phr->SetMaximum(dmax);
  phr->SetMinimum(0.0);
  phs->SetMaximum(0.1*dmax);
  pht->SetMinimum(1.e-6);
  pht->SetMaximum(1.0);
  phsx->SetMaximum(0.1*dmax);
  phs->SetMinimum(0.0);
  phsx->SetMinimum(0.0);
  phsg = dynamic_cast<TH1*>(phs->Clone(hnamsg.c_str()));
  manageHist(phsg);
  phsb = dynamic_cast<TH1*>(phs->Clone(hnamsb.c_str()));
  manageHist(phsb);
  if ( zmax == 0 ) {
    zmax = 4*nsample/nvin;
    cout << myname << "zmax = " << zmax << endl;
    for ( TH1* ph : hists2d ) ph->SetMaximum(zmax); 
  }
  return 0;
}

//**********************************************************************

TH1* AdcSampleAnalyzer::
createManagedHistogram(Name name, Name title,
                      Index nx, double xmin, double xmax,
                      Index ny, double ymin, double ymax) {
  TH1* ph = nullptr;
  Name shnam = name;
  TH1* phold = dynamic_cast<TH1*>(gDirectory->Get(shnam.c_str()));
  Index iver = 0;
  while ( phold != nullptr ) {
    ostringstream sshnam;
    sshnam << name << "_v" << ++iver;
    shnam = sshnam.str();
    phold = dynamic_cast<TH1*>(gDirectory->Get(shnam.c_str()));
  }
  if ( ny > 0 ) {
    ph = new TH2F(shnam.c_str(), title.c_str(), nx, xmin, xmax, ny, ymin, ymax);
  } else {
    ph = new TH1F(shnam.c_str(), title.c_str(), nx, xmin, xmax);
  }
  manageHist(ph);
  return ph;
}
//**********************************************************************

TH2* AdcSampleAnalyzer::
createManaged2dHistogram(Name name, Name title,
                         Index nx, double xmin, double xmax,
                         Index ny, double ymin, double ymax) {
  TH1* ph = createManagedHistogram(name, title, nx, xmin, xmax, ny, ymin, ymax);
  TH2* ph2 = dynamic_cast<TH2*>(ph);
  return ph2;
}

//**********************************************************************
