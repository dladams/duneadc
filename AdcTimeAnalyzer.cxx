// AdcTimeAnalyzer.cxx

#include "AdcTimeAnalyzer.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include "TH1F.h"
#include "TDatime.h"
#include "AdcCalibrationTree.h"
#include "TDirectory.h"
#include "TPaveStats.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TStyle.h"

using std::cout;
using std::endl;
using std::ostringstream;
using std::setw;
using std::setprecision;
using std::fixed;
using AdcTimeAnalyzerVector = AdcTimeAnalyzer::AdcTimeAnalyzerVector;

namespace {

double sint(double x) {
  double xarg = x < 0.0 ? x - 1.0 : x;
  return int(xarg);
}

// Histogram name substring for ADC means.
string meanName(int opt) {
  if ( opt == 1 ) return "meanres";
  if ( opt == 2 ) return "meanped";
  return "meanraw";
}

// Histogram title substring for ADC means.
string meanTitle(int opt) {
  if ( opt == 1 ) return "mean residual";
  if ( opt == 2 ) return "mean - pedestal";
  return "mean";
}

// Set stats option for a histogram.
void setStats(TH1* ph, int stats) {
  if ( ph == nullptr ) return;
  bool isBatch = gROOT->IsBatch();
  gROOT->SetBatch(true);
  TCanvas* pcan = new TCanvas;
  ph->Draw();
  pcan->Update();
  delete pcan;
  gROOT->SetBatch(isBatch);
  TPaveStats* psta = dynamic_cast<TPaveStats*>(ph->GetListOfFunctions()->FindObject("stats"));
  if ( psta != nullptr ) psta->SetOptStat(stats);
}

} // end unnamed namespace

//**********************************************************************

AdcTimeAnalyzerVector
AdcTimeAnalyzer::create(Name a_dsname, Index a_chip, Index nchan) {
  AdcTimeAnalyzerVector out;
  for ( Index icha=0; icha<nchan; ++icha ) {
    out.emplace_back(new AdcTimeAnalyzer(a_dsname, a_chip, icha));
  }
  return out;
}

//**********************************************************************

AdcTimeAnalyzer::AdcTimeAnalyzer(Name a_dsname, Index a_chip, Index a_chan, int dbg) :
  dsname(a_dsname), chip(a_chip), chan(a_chan) {
  const Name myname = "AdcTimeAnalyzer::ctor: ";
  double badThresh = 1.0;
  std::vector<TH1**> pphs = {&phbad, &phgood, &phlostat};
  std::vector<string> namesufs = {"bad", "good", "lostat"};
  std::vector<string> titlpres = {"Bad", "Good", "Low-stat"};
  for ( Index ihst=0; ihst<pphs.size(); ++ihst ) {
    ostringstream sshname;
    ostringstream sshtitl;
    sshname << "h_" << dsname << "_chip" << chip << "_chan" << chan << "_" << namesufs[ihst];
    sshtitl << titlpres[ihst] << " bin multiplicity for " << dsname << " chip " << chip
            << " channel " << chan
            << "; ADC bin; # samples";
    Name hname = sshname.str();
    Name htitl = sshtitl.str();
    (*pphs[ihst]) = new TH1F(hname.c_str(), htitl.c_str(), nadc, 0, nadc);
  }
  Name fname = "calib_" + dsname + ".root";
  pact = new AdcCalibrationTree(fname);
  if ( pact->status() ) {
    cout << myname << "Unable to find tree in " << fname << endl;
    cout << myname << "Status is " << pact->status() << endl;
    return;
  }
  Index nent = pact->size();
  if ( dbg > 1 ) cout << myname << "Tree entry count: " << nent << endl;
  Index ient = 0;
  ndst = 0;
  Index nbadbin = 0;
  binCountTotals.resize(nadc+1, 0);
  for ( ; ient < nent; ++ient ) {
    const AdcChannelCalibration* pcal = pact->find(chip, chan, ient);
    if ( pcal == nullptr ) continue;
    calibEntries.push_back(ient);
    sampleCountTotals.push_back(0);
    TDatime dt(pcal->time);
    if ( dbg > 1 ) dt.Print();
    for ( Index iadc=1; iadc<iadc2; ++iadc ) {
      double rms = pcal->calRmss[iadc];
      if ( rms > badThresh ) phbad->Fill(iadc);
      else if ( rms < 0.0 ) phlostat->Fill(iadc);
      else phgood->Fill(iadc);
      SampleIndex count = pcal->calCounts[iadc];
      sampleCountTotals[ndst] += count;
      binCountTotals[iadc] += count;
    }
    //cout << myname << "Found channel " << pcal->chan << ": time is " << pcal->time << endl;
    ++ndst;
  }
  if ( dbg > 0 ) cout << myname << "# datasets: " << ndst << endl;
  report(dbg > 1);
  if ( dbg > 2 ) phbad->Draw();
}

//**********************************************************************

AdcTimeAnalyzer::~AdcTimeAnalyzer() {
  delete pact;
  pact = nullptr;
}

//**********************************************************************

int AdcTimeAnalyzer::report(bool verbose) {
  const Name myname = "AdcTimeAnalyzer::report: ";
  if ( phbad == nullptr ) return 1;
  if ( ndst == 0 ) return 2;
  if ( ! m_haveReport ) {
    m_haveReport = true;
    for ( Index iadc=iadc1; iadc<iadc2; ++iadc ) {
      Index ngood = nGoodSamples(iadc);
      Index nbad = nBadSamples(iadc);
      Index nlostat = nLowStatSamples(iadc);
      Index nchk = ngood + nbad + nlostat;
      if ( nchk != ndst ) {
        cout << "ERROR: Inconsistent quality counts for bin " << iadc << ": "
             << ngood << " + " << nbad << " + " << nlostat << " != " << ndst << endl;
        continue;
      }
      Index cdst = nbad + nlostat;
      if ( cdst == 0 ) agoodBins.push_back(iadc);
      else if ( ngood == 0 ) abadBins.push_back(iadc);
      else varyBins.push_back(iadc);
      if ( nbad > 0 && ngood > 0 ) gbvaryBins.push_back(iadc);
      // Bin is declared good if it is never bad and has good stats at least once.
      if ( nbad == 0 && ngood > 0 ) goodBins.push_back(iadc);
      else badBins.push_back(iadc);
    }
  }
  Index binCount = iadc2 - iadc1;
  if ( verbose ) {
    cout << myname << "Channel " << chan << endl;
    cout << myname << "    # always good: " << setw(4) << agoodBins.size() << " ("
         << setw(5) << setprecision(1) << fixed << 100.0*double(agoodBins.size())/binCount << "%)"
         << endl;
    cout << myname << "     # always bad: " << setw(4) << abadBins.size() << " ("
         << setw(5) << setprecision(1) << fixed << 100.0*double(abadBins.size())/binCount << "%)"
        
        << endl;
    cout << myname << "           # vary: " << setw(4) << varyBins.size() << " ("
         << setw(5) << setprecision(1) << fixed << 100.0*double(varyBins.size())/binCount << "%)"
         << endl;
    cout << myname << "  # good-bad vary: " << setw(4) << gbvaryBins.size() << " ("
         << setw(5) << setprecision(1) << fixed << 100.0*double(gbvaryBins.size())/binCount << "%)"
         << endl;
    cout << myname << "  # declared good: " << setw(4) << goodBins.size() << " ("
         << setw(5) << setprecision(1) << fixed << 100.0*double(goodBins.size())/binCount << "%)"
         << endl;
    cout << myname << "   # declared bad: " << setw(4) << badBins.size() << " ("
         << setw(5) << setprecision(1) << fixed << 100.0*double(badBins.size())/binCount << "%)"
         << endl;
    // Calculate count-weighted efficiencies.
    SampleIndex nSamAllGood = 0;
    SampleIndex nSamGood = 0;
    SampleIndex nSamBad  = 0;
    for ( Index iadc : agoodBins ) nSamAllGood += binCountTotals[iadc];
    for ( Index iadc :  goodBins ) nSamGood += binCountTotals[iadc];
    for ( Index iadc :   badBins ) nSamBad  += binCountTotals[iadc];
    double den = nSamGood + nSamBad;
    double effall = double(nSamAllGood)/den;
    double eff = double(nSamGood)/den;
    cout << myname << " Count-weighted all-good efficiency: "
         << setw(5) << setprecision(1) << fixed << 100.0*effall << "%" << endl;
    cout << myname << " Count-weighted declared efficiency: "
         << setw(5) << setprecision(1) << fixed << 100.0*eff << "%" << endl;
  }
  return 0;
}

//**********************************************************************

TH1* AdcTimeAnalyzer::histMean(Index iadc, int opt, bool draw, int dbg) const {
  const Name myname = "AdcTimeAnalyzer::histMean: ";
  bool subtractLinear = opt == 1;
  bool subtractPedestal = opt == 2;
  Index iadcPedestal = 1000;
  bool subtractSomething = subtractLinear || subtractPedestal;
  double vinped = 300.0;
  ostringstream sshname;
  if ( subtractLinear ) sshname << "res";
  if ( subtractPedestal ) sshname << "ped";
  sshname << meanName(opt) << "_" << dsname << "_chip" << chip << "_chan" << chan << "_bin" << iadc;
  ostringstream sshtitl;
  sshtitl << "Ultimate " << meanTitle(opt) << " for " << dsname << " chip " << chip
          << " channel " << chan << " bin " << iadc
          << "; ";
  if ( subtractLinear ) sshtitl << "#Delta";
  sshtitl << "<V_{in}> [mV]; # entries";
  Name hname = sshname.str();
  Name htitl = sshtitl.str();
  // First collect the values.
  vector<double> vals;
  double valmin = 0.0;
  double valmax = 10.0;
  for ( Index idst=0; idst<ndst; ++idst) {
    Index ient = calibEntries[idst];
    const AdcChannelCalibration* pcal = pact->find(ient);
    if ( pcal == nullptr ) continue;
    double val = pcal->calMeans[iadc];
    if ( dbg ) cout << myname << "Mean: " << val << endl;
    if ( val < -10.0 ) continue;
    if  ( subtractLinear ) {
      double valLinear = pcal->gain*iadc + pcal->offset;
      val -= valLinear;
      if ( dbg ) cout << myname << "Residual: " << val << endl;
    }
    if  ( subtractPedestal ) {
      double valPedestal = pcal->calMeans[iadcPedestal];
      val -= valPedestal;
      if ( dbg ) cout << myname << "After ped: " << val << endl;
    }
    if ( vals.size() == 0 ) {
      valmin = val;
      valmax = val;
    }
    if ( val > valmax ) valmax = val;
    if ( val < valmin ) valmin = val;
    vals.push_back(val);
  }
  // Choose histo limits using values if range < 10 mV. Otherwise autoscale.
  double x1 = 0.0;
  double x2 = -1.0;
  double valdif = valmax - valmin;
  double valmean = 0.5*(valmin + valmax);
  double wval = subtractSomething ? 3.0 : 10.0;
  int nx = subtractSomething ? 60 : 50;
  double val1 = sint(valmean - 0.5*wval + 0.5);
  double val2 = val1 + wval;
  if ( valmin >= val1 && valmax < val2 ) {
    x1 = val1;
    x2 = val2;
  } else {
    x1 = sint(val1);
    x2 = sint(val2 + 1);
  }
  // Fill and draw histogram.
  TH1* ph = new TH1F(hname.c_str(), htitl.c_str(), nx, x1, x2);
  //ph->SetStats(0);
  ph->SetLineWidth(2);
  for ( double val : vals ) ph->Fill(val);
  if ( draw ) ph->Draw("hist");
  return ph;
}

//**********************************************************************

TH1* AdcTimeAnalyzer::histRms(Index iadc, bool draw, int dbg) const {
  const Name myname = "AdcTimeAnalyzer::histRms: ";
  if ( iadc >= nadc ) return nullptr;
  if ( m_rmsHists.size() < nadc ) m_rmsHists.resize(nadc);
  HistPtr& ph = m_rmsHists[iadc];
  if ( ph == nullptr ) {
    ostringstream sshname;
    sshname << "hrms_" << dsname << "_chip" << chip << "_chan" << chan << "_bin" << iadc;
    ostringstream sshtitl;
    sshtitl << "Ultimate RMS for " << dsname << " chip " << chip
            << " channel " << chan << " bin " << iadc
            << "; RMS [mV]; # entries";
    Name hname = sshname.str();
    Name htitl = sshtitl.str();
    double rmsmax = 2.0;
    ph.reset(new TH1F(hname.c_str(), htitl.c_str(), 40, 0, rmsmax));
    ph->SetDirectory(nullptr);
    //ph->SetStats(0);
    ph->SetLineWidth(2);
    for ( Index idst=0; idst<ndst; ++idst) {
      Index ient = calibEntries[idst];
      const AdcChannelCalibration* pcal = pact->find(ient);
      if ( pcal == nullptr ) continue;
      double rms = pcal->calRmss[iadc];
      if ( dbg ) cout << myname << "RMS: " << rms << endl;
      if ( rms > 0.0 ) {
        ph->Fill(rms);
      }
    }
  }
  if ( draw ) ph->Draw("hist");
  return &*ph;
}

//**********************************************************************

TH1* AdcTimeAnalyzer::histRmsTime(Index iadc, bool draw) const {
  ostringstream sshname;
  sshname << "hrmst_" << dsname << "_chip" << chip << "_chan" << chan << "_bin" << iadc;
  ostringstream sshtitl;
  sshtitl << "Ultimate RMS for " << dsname << " chip " << chip
          << " channel " << chan << " bin " << iadc
          << "; Time index; RMS [mV]";
  Name hname = sshname.str();
  Name htitl = sshtitl.str();
  double rmsmax = 2.0;
  TH1* ph = new TH1F(hname.c_str(), htitl.c_str(), ndst, 0, ndst);
  ph->SetStats(0);
  ph->SetLineWidth(2);
  ph->SetMinimum(0.0);
  ph->SetMaximum(rmsmax);
  for ( Index idst=0; idst<ndst; ++idst) {
    Index ient = calibEntries[idst];
    const AdcChannelCalibration* pcal = pact->find(ient);
    if ( pcal == nullptr ) continue;
    double rms = pcal->calRmss[iadc];
    if ( rms >= rmsmax ) rms = 0.999*rmsmax;
    if ( rms < 0 ) rms = 0.0;
    ph->SetBinContent(idst+1, rms);
  }
  if ( draw ) ph->Draw("hist");
  return ph;
}

//**********************************************************************

TH1* AdcTimeAnalyzer::histCount(Index iadc, bool draw, int dbg) const {
  const Name myname = "AdcTimeAnalyzer::histCount: ";
  if ( iadc >= nadc ) return nullptr;
  if ( m_countHists.size() < nadc ) m_countHists.resize(nadc);
  HistPtr& ph = m_countHists[iadc];
  if ( ph == nullptr ) {
    ostringstream sshname;
    sshname << "hcount_" << dsname << "_chip" << chip << "_chan" << chan << "_bin" << iadc;
    ostringstream sshtitl;
    sshtitl << "Sample count for " << dsname << " chip " << chip
            << " channel " << chan << " bin " << iadc
            << "; Count; # entries";
    Name hname = sshname.str();
    Name htitl = sshtitl.str();
    ph.reset(new TH1F(hname.c_str(), htitl.c_str(), 40, 0, -1));
    ph->SetDirectory(nullptr);
    //ph->SetStats(0);
    ph->SetLineWidth(2);
    for ( Index idst=0; idst<ndst; ++idst) {
      Index ient = calibEntries[idst];
      const AdcChannelCalibration* pcal = pact->find(ient);
      if ( pcal == nullptr ) continue;
      SampleIndex count = pcal->calCounts[iadc];
      if ( dbg ) cout << myname << "Count: " << count << endl;
      ph->Fill(count);
    }
  }
  if ( draw ) ph->Draw("hist");
  return &*ph;
}

//**********************************************************************

TH1* AdcTimeAnalyzer::histCountTime(Index iadc, bool draw) const {
  ostringstream sshname;
  sshname << "hcountt" << dsname << "_chip" << chip << "_chan" << chan << "_bin" << iadc;
  ostringstream sshtitl;
  sshtitl << "Sample count RMS for " << dsname << " chip " << chip
          << " channel " << chan << " bin " << iadc
          << "; Time index; Count";
  Name hname = sshname.str();
  Name htitl = sshtitl.str();
  double rmsmax = 2.0;
  TH1* ph = new TH1F(hname.c_str(), htitl.c_str(), ndst, 0, ndst);
  ph->SetStats(0);
  ph->SetLineWidth(2);
  ph->SetMinimum(0.0);
  //ph->SetMaximum(1.03*rmsmax);
  for ( Index idst=0; idst<ndst; ++idst) {
    Index ient = calibEntries[idst];
    const AdcChannelCalibration* pcal = pact->find(ient);
    if ( pcal == nullptr ) continue;
    double count = pcal->calCounts[iadc];
    ph->SetBinContent(idst+1, count);
  }
  if ( draw ) ph->Draw("hist");
  return ph;
}

//**********************************************************************

TH1* AdcTimeAnalyzer::histMeanRms(int opt, bool draw, int dbg) {
  const Name myname = "AdcTimeAnalyzer::histMeanRms: ";
  ostringstream sshname;
  sshname << meanName(opt) << dsname << "_chip" << chip << "_chan" << chan;
  ostringstream sshtitl;
  sshtitl << "Ultimate " << meanTitle(opt) << " RMS for " << dsname << " chip " << chip
          << " channel " << chan
          << "; Count; # entries";
  Name hname = sshname.str();
  Name htitl = sshtitl.str();
  double rmsmax = 2.0;
  TH1* ph = new TH1F(hname.c_str(), htitl.c_str(), 100, 0, 2.0);
  ph->SetLineWidth(2);
  //setStats(ph, 101110);
  for ( Index iadc=iadc1; iadc<iadc2; ++iadc) {
    if ( dbg == 1 && 100*(iadc/100) == iadc ) cout << myname << iadc << endl;
    TH1* phm = histMean(iadc, opt, 0);
    double rms = phm->GetRMS();
    if ( rms >= rmsmax ) rms = 0.999*rmsmax;
    ph->Fill(phm->GetRMS());
    gDirectory->Delete(phm->GetName());
  }
  if ( draw ) {
    int istats = gStyle->GetOptStat();
    gStyle->SetOptStat(101110);
    ph->Draw("hist");
    gPad->Update();
    gStyle->SetOptStat(istats);
  }
  return ph;
}

//**********************************************************************

Index AdcTimeAnalyzer::nGoodSamples(Index iadc) const {
  if ( phgood == nullptr ) return 0;
  if ( iadc > Index(phgood->GetNbinsX()) ) return 0;
  return phgood->GetBinContent(iadc+1);
}

//**********************************************************************

Index AdcTimeAnalyzer::nBadSamples(Index iadc) const {
  if ( phbad == nullptr ) return 0;
  if ( iadc > Index(phbad->GetNbinsX()) ) return 0;
  return phbad->GetBinContent(iadc+1);
}

//**********************************************************************

Index AdcTimeAnalyzer::nLowStatSamples(Index iadc) const {
  if ( phlostat == nullptr ) return 0;
  if ( iadc > Index(phlostat->GetNbinsX()) ) return 0;
  return phlostat->GetBinContent(iadc+1);
}

//**********************************************************************

