// AdcSampleAnalyzer.cxx

#include "AdcSampleAnalyzer.h"
#include <string>
#include <iostream>
#include <sstream>
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

AdcSampleAnalyzer::AdcSampleAnalyzer(const AdcSampleReader& rdr, string adatasetCalib, double a_nominalGain)
: datasetCalib(adatasetCalib),
  pfit(nullptr), fitGain(0.0), fitOffset(0.0),
  nominalGain(a_nominalGain),
  m_preader(&rdr),
  m_dataset(rdr.dataset()),
  m_sampleName(rdr.sample()),
  m_refCalib(m_localCalib),
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
  cout << myname << "Processing sample " << rdr.sample() << endl;
  nsample = rdr.nsample();
  localCalib().chip = rdr.chip();
  localCalib().chan = channel();
  localCalib().time = rdr.time();
  haveNominalCalibration = datasetCalib.size() && (datasetCalib != "none");
  if ( haveNominalCalibration ) {
    nominalCalibrationIsLinear = datasetCalib == "linear";
    if ( nominalCalibrationIsLinear ) {
      if ( nominalGain <= 0.0 ) {
        cout << myname << "ERROR: Invalid nominal gain: " << a_nominalGain << endl;
        return;
      }
      cout << myname << "Nominal calibration is linear with gain " << nominalGain << "  mV/ADC." << endl;
    } else {
      cout << myname << "Taking nominal calibration from dataset " << datasetCalib << endl;
      pcalNominal = AdcChannelCalibration::find(datasetCalib, chip(), channel());
      if ( pcalNominal == nullptr ) {
        cout << myname << "ERROR: Nominal calibration not found for dataset " << datasetCalib << endl;
        return;
      }
    }
  } else {
    cout << myname << "Nominal calibration is not provided." << endl;
  }
  // DCreate the histograms.
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
  localCalib().gain = fitGain;
  localCalib().offset = fitOffset;
  cout << myname << "Fit gain: " << fitGain << " mV/ADC, offset: " << fitOffset << " mV" << endl;
  ostringstream ssdif;
  ssdif.precision(3);
  ssdif << "V_{in} - (" << fitGain << " ADC + " << fitOffset << ") [mV]";
  phd->GetYaxis()->SetTitle(ssdif.str().c_str());
  phdw->GetYaxis()->SetTitle(ssdif.str().c_str());
  // Find the nominal calibration.
  // If datasetCalib is set, use its calibration for this channel.
  // Otherwise, use the gain from the reader and the pedestal from data at ADC = 500.
  if ( pcalNominal != nullptr ) {
    ostringstream ssdifn;
    ssdifn.precision(3);
    ssdifn << "V_{in} - V_{in}(" << datasetCalib << ") [mV]";
    phn->GetYaxis()->SetTitle(ssdifn.str().c_str());
    phnw->GetYaxis()->SetTitle(ssdifn.str().c_str());
    phvn->GetYaxis()->SetTitle(ssdifn.str().c_str());
  }
  // Find the offset for a linear calbration.
  // This uses the linear calibration of this data.
  if ( nominalCalibrationIsLinear ) {
    Index adc_nom = 500;
    cout << myname << "Nominal gain taken from reader, offset from data at ADC = " << adc_nom << endl;
    nominalOffset = fitOffset + (fitGain - nominalGain)*adc_nom;
    cout << myname << "Nominal gain: " << nominalGain << " mV/ADC, offset: "
         << nominalOffset << " mV" << endl;
    ostringstream ssdifn;
    ssdifn.precision(3);
    ssdifn << "V_{in} - (" << nominalGain << " ADC + " << nominalOffset << ") [mV]";
    phn->GetYaxis()->SetTitle(ssdifn.str().c_str());
    phnw->GetYaxis()->SetTitle(ssdifn.str().c_str());
    phvn->GetYaxis()->SetTitle(ssdifn.str().c_str());
  }
  // Fill the remaining histograms.
  vector<Index> nsamtot;
  vector<Index> nsamkeep;
  for ( Index iadc=0; iadc<nadc(); ++iadc ) {
    double evinCalib = haveNominalCalibration ? nominalCalibrationVin(iadc) : 0.0;
    double ermsCalib = pcalNominal == nullptr ? 0.0 : nominalCalibrationRms(iadc);
    for ( Index ivin=0; ivin<nvin; ++ivin ) {
      double vin = rdr.vinCenter(ivin);
      double evinLinear = fitOffset + iadc*fitGain;
      Index count = rdr.countTable()[iadc][ivin];
      phd->Fill( iadc, vin - evinLinear, count);
      phdw->Fill(iadc, vin - evinLinear, count);
      if ( haveNominalCalibration ) {
        phn->Fill( iadc, vin - evinCalib,  count);
        phnw->Fill(iadc, vin - evinCalib,  count);
        if ( ermsCalib < 1.0 ) phvn->Fill(vin, vin - evinCalib,  count);
      }
    }
  }
  // Fill diff stat histograms.
  localCalib().calMeans.resize(nadc());
  localCalib().calRmss.resize(nadc());
  localCalib().calCounts.resize(nadc());
  localCalib().calTails.resize(nadc());
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
    localCalib().calCounts[iadc] = count;
    localCalib().calMeans[iadc] = xm + xmLinear;
    localCalib().calRmss[iadc] = xs;
    localCalib().calTails[iadc] = tailfrac;
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
    if ( haveNominalCalibration ) {
      ph = hdiffn(iadc);
      xm = ph->GetMean();
      xs = ph->GetRMS();
      xr = sqrt(xm*xm+xs*xs);
      if ( iadc > 64 && !isStuck ) {
        phdn->Fill(xr);
      }
    }
  }
  if ( true ) {
    ProcInfo_t info;
    gSystem->GetProcInfo(&info);
    cout << myname << "Virtual memory at end of ctor is " << info.fMemVirtual/1000000.0 << " GB" << endl;
  }
  return;
}

//**********************************************************************

AdcSampleAnalyzer::AdcSampleAnalyzer(AdcSampleReaderPtr preader, string adatasetCalib, double cfac)
: AdcSampleAnalyzer(*preader, adatasetCalib, cfac) {
  m_preaderManaged.swap(preader);
}

//**********************************************************************

AdcSampleAnalyzer::
AdcSampleAnalyzer(Name a_dataset, Name a_sampleName,
                  const AdcChannelCalibration& a_calib)
: m_dataset(a_dataset), m_sampleName(a_sampleName), m_refCalib(a_calib),
  m_chip(calib().chip),
  m_channel(calib().chan),
  m_time(calib().time),
  m_nadc(calib().calMeans.size()) {
  fitGain = calib().gain;
  fitOffset = calib().offset;
  adcUnderflow = 64;
  createHistograms(2000, 0, 2000);
  for ( Index iadc=0; iadc<nadc(); ++iadc ) {
    double fitMean = fitGain*iadc + fitOffset;
    double calMean = calib().calMeans[iadc];
    double xm = calMean - fitMean;
    double xs = calib().calRmss[iadc];
    bool isStuck = sticky(iadc);
    double xsg = isStuck ? 0 : xs;
    double xsb = !isStuck ? 0 : xs;
    double xr = sqrt(xm*xm+xs*xs);
    phm->SetBinContent(iadc+1, xm);
    phs->SetBinContent(iadc+1, xs);
    phsg->SetBinContent(iadc+1, xsg);
    phsb->SetBinContent(iadc+1, xsb);
    phr->SetBinContent(iadc+1, xr);
    pht->SetBinContent(iadc+1, calib().calTails[iadc]);
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
}

//**********************************************************************

void AdcSampleAnalyzer::clean() {
  // Delete the histograms managed locally.
  for ( TH1* ph : m_localHists ) delete ph;
  m_localHists.clear();
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
  m_localHists.push_back(ph);
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
  m_localHists.push_back(ph);
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
  m_localHists.push_back(ph);
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
    m_localHists.push_back(ph);
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
  if ( nominalCalibrationIsLinear ) {
    return nominalOffset + iadc*nominalGain;
  }
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
  cout << myname << "ERROR: Sample analyzer does not have a nominal calibration RMS." << endl;
  return 0.0;
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
  AdcChannelId id(chip(), channel());
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
AdcSampleAnalyzer::evaluateVoltageEfficiencies(double rmsmax, bool readData, bool dropTails) {
  const string myname = "AdcSampleAnalyzer::evaluateVoltageEfficiencies: ";
  static FloatVector empty;
  if ( reader() == nullptr ) {
    cout << myname << "ERROR: Reader has been removed." << endl;
    return empty;
  }
  const AdcSampleReader& rdr = *reader();
  evaluateReadData = readData;
  unsigned int nvr = voltageResponses.size();
  if ( phc ==  nullptr ) return empty;
  if ( nvr < 1 ) return empty;
  // Create voltage performance object.
  double v1 = voltageResponses[0].vmin;
  double v2 = voltageResponses[nvr-1].vmax;
  vperfs.emplace_back(chip(), channel(), time(), rmsmax, nvr, v1, v2);
  AdcVoltagePerformance& vperf = vperfs.back();
  // Create efficiency histogram.
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
  phvrms->SetMaximum(rmsmax);
  phvrms->SetLineWidth(2);
  // Create tail histogram.
  sshnam.str("");
  sshnam << "hvtail" << rmsmax;
  shnam = sshnam.str();
  sstitl.str("");
  sstitl << phc->GetTitle();
  sstitl << " tail fraction for RMS < " << rmsmax << " mV";
  sstitl << ";V_{in} [mV]";
  sstitl << ";Tail fraction";
  stitl = sstitl.str();
  for ( char& ch : shnam ) if ( ch == '.' ) ch = 'p';
  phvtail = new TH1F(shnam.c_str(), stitl.c_str(), nvr, v1, v2);
  phvtail->SetStats(0);
  phvtail->SetMinimum(1.e-6);
  phvtail->SetMaximum(1.0);
  phvtail->SetLineWidth(2);
  // Fill voltage performance and histograms.
  double dv = (v2 - v1)/nvr;
  double dvhalf = 0.5*dv;
  FloatVector gx(nvr, 0.0);
  FloatVector gy(nvr, 0.0);
  FloatVector gexlo(nvr, dvhalf);
  FloatVector gexhi(nvr, dvhalf);
  FloatVector geylo(nvr, 0.0);
  FloatVector geyhi(nvr, 0.0);
  FloatVector gey10(nvr, 0.0);
  FloatVector gey90(nvr, 0.0);
  Index ictvin = 0;                 // Voltage index for the reader count table
  Index nctvin = rdr.nvin();     // Maximum voltage index for the reader count table
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
    double eff = 0.0;
    double rmsMean = 0.0;
    double rmsRms = 0.0;
    double rmsRmslo = 0.0;
    double rmsRmshi = 0.0;
    double rmsRms10 = 0.0;
    double rmsRms90 = 0.0;
    double tailFrac = 0.0;
    // Evaluate efficiency, resolution and tail fraction using the input data.
    if ( readData ) {
      double effSum = 0.0;
      double devSum = 0.0;
      double dev2Sum = 0.0;
      ostringstream sshnam;
      sshnam << "hdvin2_" << ivr;
      string shnam = sshnam.str();
      TH1* phdvin2 = new TH1F(shnam.c_str(), "", 1000, 0, rmsmax);   // Histogram used to get RMS and RMS extent
      if ( remhist ) phdvin2->SetDirectory(0);
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
          double dvin = vinMeasured - vinTrue;
          double dvin2 = dvin*dvin;
          double apull = fabs(dvin/rmsMeasured);
          double advin = fabs(dvin);
          bool skipBin = false;
          skipBin |= rmsMeasured < 0.0;             // Bin has too few measurements
          skipBin |= rmsMeasured >= rmsmax;         // Bin RMS is too large
          skipBin |= dropTails && tailFrac > 0.0;   // Drop bins with tails
          if ( ! skipBin ) {      // RMS < 0 is uncalibrated bin treated here as bad
            //phdvin2->Fill(dvin2, count);
            phdvin2->Fill(rmsMeasured, count);
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
        double sum = 0.0;
        double dvin2_0 = 0.0;
        double dvin2_10 = 0.0;
        double dvin2_90 = 0.0;
        double dvin2_100 = 0.0;
        int lastbin = phdvin2->GetNbinsX()+1;
        for ( int bin=0; bin<=lastbin; ++bin ) {
          double count = phdvin2->GetBinContent(bin);
          sum += count;
          double x = phdvin2->GetXaxis()->GetBinCenter(bin);
          if ( bin == 0 ) x = 0.0;
          if ( bin == lastbin ) x = rmsmax;
          if ( sum == count ) dvin2_0 = x;
          if ( sum <= 0.1*effSum ) dvin2_10 = x;
          if ( sum <= 0.9*effSum ) dvin2_90 = x;
          if ( count > 0 ) dvin2_100 = x;
        }
        eff = effSum/countSum;
        double devMean = devSum/effSum;
        double dev2Mean = dev2Sum/effSum;
        rmsMean = sqrt(dev2Mean);        // RMS deviation
        rmsRms = phdvin2->GetMean();     // Store the expected mean RMS
        rmsRmslo = dvin2_0;
        rmsRmshi = dvin2_100;
        rmsRms10 = dvin2_10;
        rmsRms90 = dvin2_90;
        tailFrac = tailFracSum/effSum;
      }
      if ( remhist ) delete phdvin2;
    // Evaluate efficiency, resolution and tail fraction using the calibration.
    } else {
      double rmsSum = 0.0;
      double rms2Sum = 0.0;
      double losum = 0.0;
      double hisum = 0.0;
      double drmslo2Sum = 0.0;
      double drmshi2Sum = 0.0;
      // Loop twice over ADC bins finding the mean on the first pass.
      for ( int haveMean=0; haveMean<2; ++haveMean ) {
        for ( unsigned int iadc=iadc1; iadc<iadc2; ++iadc ) {
          countSum += avr.count(iadc);
          double rms = calRms(iadc);
          if ( rms >= 0.0 && rms < rmsmax ) {      // RMS < 0 is uncalibrated bin treated here as bad
            double frac = avr.fraction(iadc);
            if ( ! haveMean ) {
              eff += frac;
              rmsSum += frac*rms;
              rms2Sum += frac*rms*rms;
              double tFrac = calTail(iadc);
              tailFrac += frac*tFrac;
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
        if ( ! haveMean ) {
          rmsMean = eff > 0.0 ? rmsSum/eff : 0.0;
        } else {
          double rms2Mean = eff > 0.0 ? rms2Sum/eff : 0.0;
          double diff = rms2Mean - rmsMean*rmsMean;
          if ( diff < 0.0 ) diff = 0.0;
          rmsRms = sqrt(diff);
          rmsRmslo = losum > 0 ? sqrt(drmslo2Sum/losum) : 0.0;
          rmsRmshi = hisum > 0 ? sqrt(drmshi2Sum/hisum) : 0.0;
        }
      }
    }
    vperf.vinCounts[ivr] = countSum;
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
    phvtail->SetBinContent(ivr+1, tailFrac);
    gx[ivr] = v1 + dvhalf + ivr*dv;
    gy[ivr] = rmsMean;
    geylo[ivr] = rmsMean - rmsRmslo;
    geyhi[ivr] = rmsRmshi - rmsMean;
    gey10[ivr] = rmsRms10;
    gey90[ivr] = rmsRms90;
  }
  pgvrms = new TGraphAsymmErrors(nvr, &gx[0], &gy[0], &gexlo[0], &gexhi[0], &geylo[0], &geyhi[0]);
  g80bars.clear();
  for ( unsigned int ivr=0; ivr<nvr; ++ivr ) {
    TLine* pline = new TLine(gx[ivr], gey10[ivr], gx[ivr], gey90[ivr]);
    pline->SetLineWidth(3);
    g80bars.push_back(pline);
  }
  //pgvrms->SetLineWidth(2);
  return vperf.vinEffs;
}

//**********************************************************************

void AdcSampleAnalyzer::drawperf(bool dolabtail) const {
  if ( phveff == nullptr ) return;
  if ( phvtail == nullptr ) return;
  if ( pgvrms == nullptr ) return;
  double ymax = 1.03;
  double rmsmax = phvrms->GetMaximum();
  if ( rmsmax > ymax ) ymax = rmsmax;
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
  //TCanvas* pcan = new TCanvas;
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
  pgvrms->Draw("Z");
  //pgvrms->SetLineWidth(2);
  //pgvrms->Draw("XL");
  for ( TLine* pline : g80bars ) pline->Draw();
  TLegend* pleg = new TLegend(0.3, 0.73, 0.45, 0.87);
  pleg->SetBorderSize(0);
  pleg->SetFillStyle(0);
  pleg->AddEntry(phax, "Efficiency", "l");
  pleg->AddEntry(pgvrms, "Resolution", "le");
  pleg->AddEntry(phts, "Tail fraction", "f");
  pleg->Draw();
  TLine* pline = new TLine(x1, 0.0, x2, 0.0);
  pline->Draw();
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
  phf = new TH2F(hnamf.c_str(), stitle.c_str(), npadc, 0, padcmax, npvin, pvinmin, pvinmax);
  phc = new TH2F(hnamc.c_str(), stitle.c_str(), npadc, 0, padcmax, npvin, pvinmin, pvinmax);
  phd = new TH2F(hnamd.c_str(), stitle.c_str(), npadc, 0, padcmax, 400, -dmax, dmax);
  phdw = new TH2F(hnamdw.c_str(), stitle.c_str(), npadc, 0, padcmax, 400, -wdmax, wdmax);
  phm = new TH1F(hnamm.c_str(), stitlm.c_str(), npadc, 0, padcmax);
  phr = new TH1F(hnamr.c_str(), stitlr.c_str(), npadc, 0, padcmax);
  phs = new TH1F(hnams.c_str(), stitls.c_str(), npadc, 0, padcmax);
  pht = new TH1F(hnamt.c_str(), stitlt.c_str(), npadc, 0, padcmax);
  phsx = new TH1F(hnamsx.c_str(), stitlsx.c_str(), npadc, 0, padcmax);
  phdr = new TH1F(hnamdr.c_str(), stitle.c_str(), nd, 0, dmax);
  phds = new TH1F(hnamds.c_str(), stitle.c_str(), nd, 0, smax);
  phdsb = new TH1F(hnamdsb.c_str(), stitldsb.c_str(), nd, 0, smax);
  vector<TH1*> hists2d = {phf, phc, phd};
  vector<TH1*> dhists = {phdr, phds, phdsb, phdw};
  // Add calibration histograms.
  if ( haveNominalCalibration ) {
    phn = new TH2F(hnamn.c_str(), stitle.c_str(), npadc, 0, padcmax, 400, -dmax, dmax);
    phnw = new TH2F(hnamnw.c_str(), stitle.c_str(), npadc, 0, padcmax, 400, -wdmax, wdmax);
    phvn = new TH2F(hnamvn.c_str(), stitle.c_str(), nvinperf, vinperfmin, vinperfmax, 400, -dmax, dmax);
    phdn = new TH1F(hnamdn.c_str(), stitle.c_str(), nd, 0, wdmax);
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
  phsb = dynamic_cast<TH1*>(phs->Clone(hnamsb.c_str()));
  if ( zmax == 0 ) {
    zmax = 4*nsample/nvin;
    cout << myname << "zmax = " << zmax << endl;
    for ( TH1* ph : hists2d ) ph->SetMaximum(zmax); 
  }
  return 0;
}

//**********************************************************************
