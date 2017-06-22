// AdcChipAnalyzer.cxx

#include "AdcChipAnalyzer.h"
#include "AdcSampleFinder.h"
#include "AdcCalibrationTree.h"
#include "AdcPerformanceTree.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include "AdcSampleAnalyzer.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TLine.h"
#include "TStyle.h"
#include "TLegend.h"
#include "TSystem.h"
#include "TCanvas.h"

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;
using std::setw;
using std::vector;
typedef unsigned int Index;

//**********************************************************************

void AdcChipAnalyzer::help(Name prefix) {
  cout << prefix << "Display plots with aca.draw(plotName, doPrint)" << endl;
  cout << prefix << "Supported plot names:" << endl;
  cout << prefix << "   rawv - Raw waveform plus Vin." << endl;
  cout << prefix << "   resp - Inverse response (V{in} vs. ADC bin)." << endl;
  cout << prefix << "   zres - Inverse response for ADC bin < 300." << endl;
  cout << prefix << "   diff - Linear-fit residual." << endl;
  cout << prefix << "   difn - Calibration residual." << endl;
  cout << prefix << "   frms - Linear resolution (residual RMS from linear fit) vs. ADC bin." << endl;
  cout << prefix << "   fsdv - Ultimate resolution (residual RMS from ultimate calibration) vs. ADC bin." << endl;
  cout << prefix << "   fsdz - Ultimate resolution vs. ADC bin with expanded scale (5 mV)." << endl;
  cout << prefix << "   fsdg - Ultimate resolution vs. ADC bin for good bins only." << endl;
  cout << prefix << "   fsdb - Ultimate resolution vs. ADC bin for bad bins only." << endl;
  cout << prefix << "   fsdx - Expanded (pull > 5.0) ultimate resolution vs. ADC bin." << endl;
  cout << prefix << "   fsdt - Tail fraction vs. ADC bin." << endl;
  cout << prefix << "   fmea - Mean linear fit residual (ultimate calibration constants) vs. ADC bin." << endl;
  cout << prefix << "    fdr - Linear-fit resolution distribution." << endl;
  cout << prefix << "    fds - Ultimate resolution distribution." << endl;
  cout << prefix << "   fdsb - Ultimate resolution distribution for bad channels." << endl;
  cout << prefix << "   fnds - RMS distribution for the input calibration." << endl;
  cout << prefix << "   veff - Efficiency (good fraction) vs. input voltage." << endl;
  cout << prefix << "   vdev - Mean (measured-true) vs. input voltage." << endl;
  cout << prefix << "   vrms - RMS(measured - true) vs. input voltage." << endl;
  cout << prefix << "   perf - Efficiency, resolution and tail fraction vs. input voltage." << endl;
  cout << prefix << " sumfdr - Linear-fit resolution distribution summed over channels." << endl;
  cout << prefix << " sumfds - Ultimate resolution distribution summed over channels." << endl;
  cout << prefix << "   gain - Gain distribution for all channels." << endl;
  cout << prefix << " offset - Offset distribution for all channels." << endl;
}

//**********************************************************************

AdcChipAnalyzer::
AdcChipAnalyzer(string a_sampleName,
                Index a_icha1, Index a_ncha,
                string a_datasetCalib,
                bool a_saveCalib,
                float a_vmin, float a_vmax, Index a_nv,
                double a_vRmsMax,
                bool a_dropTails,
                bool a_savePerf,
                int cleanFlag)
: m_sampleName(a_sampleName),
  m_icha1(0),
  m_ncha(0),
  m_datasetCalib(a_datasetCalib),
  m_saveCalib(a_saveCalib),
  m_vmin(a_vmin),
  m_vmax(a_vmax),
  m_nv(a_nv),
  m_vRmsMax(a_vRmsMax),
  m_dropTails(a_dropTails),
  m_savePerf(a_savePerf) {
  string myname = "AdcChipAnalyzer::ctor: ";
  // Create linear-fit gain summary histogram.
  string hgtitl = sampleName() + " Linear fit gain; Gain [mV/ADC]; # channels";
  m_phGain = new TH1F("hg", hgtitl.c_str(), 60, 0, 0.6);
  m_phGain->SetDirectory(0);   // Don't delete this
  m_phGain->SetLineWidth(2);
  m_hists.push_back(m_phGain);
  // Create linear-fit offset summary histogram.
  string hptitl = sampleName() + " Linear fit offset; Offset [mV]; # channels";
  m_phOffset = new TH1F("hp", hptitl.c_str(), 40, -100, 100);
  m_phOffset->SetDirectory(0);   // Don't delete this
  m_phOffset->SetLineWidth(2);
  m_hists.push_back(m_phOffset);
  // Set channels and read data.
  setChannels(a_icha1, a_ncha, cleanFlag);
  // Draw plots.
  cout << myname << "Chip analyzer constructed from sample " << sampleName() << "." << endl;
  cout << myname << "Use this->draw(plotName) to draw plots (plotName = \"help\" to list)." << endl;
}

//**********************************************************************

AdcChipAnalyzer::~AdcChipAnalyzer() {
  for ( AdcSampleAnalyzer*& pasa : m_asas ) {
    delete pasa;
    pasa = nullptr;
  }
  for ( TH1* ph : m_hists ) delete ph;
  m_hists.clear();
  for ( TH1* ph : m_rawHists ) delete ph;
  m_rawHists.clear();
  for ( TH1* ph : m_vinHists ) delete ph;
  m_vinHists.clear();
  for ( const AdcChannelCalibration* pcal : m_pcals ) delete pcal;
  m_pcals.clear();
}

//**********************************************************************

int AdcChipAnalyzer::setChannels(Index a_icha1, Index a_ncha, int cleanFlag) {
  string myname = "AdcChipAnalyzer::setChannels: ";
  m_icha1 = a_icha1;
  m_ncha = a_ncha;
  m_chans.clear();
  // Assign channel and pad numbers.
  if ( sampleName() == "201612_ltc2314" && nChannel() > 0 ) {
    if ( nChannel() == 1 ) m_chans.push_back(firstChannel());
    else {
      m_ncha = 4;
      m_chans.push_back(0);
      m_chans.push_back(1);
      m_chans.push_back(8);
      m_chans.push_back(11);
    }
  } else {
    Index maxnchan = 16 - firstChannel();
    if ( m_ncha > maxnchan ) m_ncha = maxnchan;
    for ( Index kcha=0; kcha<nChannel(); ++kcha ) {
      Index icha = firstChannel() + kcha;
      m_chans.push_back(icha);
    }
  }
  m_clean = cleanFlag;
  if ( cleanFlag == 2 && nChannel() == 1 ) m_clean = false;
  // Fetch the analyzer for each channel.
  for ( Index kcha=0; kcha<nChannel(); ++kcha ) {
    Index icha = channel(kcha);
    AdcSampleAnalyzer* pasa = sampleAnalyzer(icha);
    if ( pasa == nullptr || pasa->phc == nullptr ) {
      cout << myname << "ERROR: Unable to analyze channel " << icha << endl;
      return 1;
    };
  }
  return 0;
}

//**********************************************************************

TCanvas* AdcChipAnalyzer::newCanvas(string splot, string canName) const {
  gStyle->SetOptStat(110111);
  Index npadx = 1;
  bool onePad = nChannel() == 1 ||
                splot.substr(0,3) == "sum" ||
                splot == "gain" ||
                splot == "offset";
  if ( ! onePad ) {
    if ( nChannel() > 4 ) {
      npadx = 4;
    } else if ( nChannel() > 1 ) {
      npadx = 2;
    }
  }
  Index npady = npadx;
  Index npad = npadx*npadx;
  Index wwx = 700;
  Index wwy = 500;
  if ( gROOT->IsBatch() && npad ) {
    wwx *= npadx;
    wwy *= npady;
  }
  TCanvas* pcan = new TCanvas(canName.c_str(), canName.c_str(), wwx, wwy);
  if ( npadx ) pcan->Divide(npadx, npadx);
  return pcan;
}

//**********************************************************************

int AdcChipAnalyzer::drawall() {
  string myname = "AdcChipAnalyzer::draw: ";
  gStyle->SetOptStat(110111);
  vector<string> splots = {"rawv", "resp", "diff", "difn", "zres", "fmea", "fsdv", "fsdx", "fsdt", "veffall"};
  //vector<string> splots = {"resp", "diff", "difn", "zres", "frms", "fsdv", "fsdz", "fsdg", "fmea", "fdn", "fdr", "fds", "fdsb", "veff"};
  //vector<string> splots = {"fdn", "fdr", "fds", "fdsb"};
  Index rstat = 0;
  for ( unsigned int ityp=0; ityp<splots.size(); ++ityp ) {
    rstat += draw(splots[ityp], true);
  }
  return rstat;
}

//**********************************************************************

int AdcChipAnalyzer::draw(string splotin, bool savePlot) {
  string myname = "AdcChipAnalyzer::draw: ";
  if ( splotin == "help" ) {
    help(myname);
    return 0;
  }
  // Fill the map of histograms that are summed over channels..
  if ( m_hsums.size() == 0 ) {
    m_hsums["fdr"] = nullptr;
    m_hsums["fds"] = nullptr;
  }
  // Build strings with channel range.
  ostringstream sschan;
  sschan << firstChannel();
  if ( nChannel() > 1 ) sschan << "-" << firstChannel() + nChannel() - 1;
  string schan = sschan.str();
  string schanlab = string(nChannel() == 1 ? "channel" : "channels") + " " + schan;
  // Suffix for object names.
  ostringstream ssnamsuf;
  ssnamsuf << splotin << "_" << firstChannel();
  if ( nChannel() > 1 ) ssnamsuf << "_" << firstChannel() + nChannel() - 1;
  string namsuf = ssnamsuf.str();
  // Create canvas for the plots.
  string canName = "can_" + namsuf;
  TCanvas* pcan = newCanvas(splotin, canName.c_str());
  m_cans.push_back(pcan);
  // Loop over channels and add plots.
  TVirtualPad* ppad = pcan;
  // Check if this is summed over channels.
  bool doChannelSum = false;
  string splot = splotin;
  if ( splot.substr(0,3) == "sum" ) {
    doChannelSum = true;
    splot = splotin.substr(3);
  }
  for ( Index kcha=0; kcha<nChannel(); ++kcha ) {
    Index icha = channel(kcha);
    if ( nChannel() ) ppad = pcan->cd(kcha + 1 );
    AdcSampleAnalyzer* pasa = sampleAnalyzer(icha);
    if ( pasa == nullptr ) {
      cout << myname << "WARNING: Skipping channel " << icha << endl;
      continue;
    }
    AdcSampleAnalyzer& asa = *pasa;
    if ( asa.phc == nullptr ) {
      cout << myname << "ERROR: Unable to analyze channel " << icha << endl;
      return 1;
    };
    if ( asa.channel() != icha ) {
      cout << myname << "ERROR: Reader channel differs from input: " << asa.channel() << " != " << icha << "." << endl;
      return 2;
    };
    TH1* ph = nullptr;
    TH1* ph2 = nullptr;
    string sarg = "hist";
    string sarg2 = "hist same";
    double rightMargin = 0.02;
    double leftMargin = 0.10;
    bool logy = false;
    bool gridx = false;
    bool gridy = false;
    bool doDraw = true;
    bool doLine = false;  // Draw a line for the x-axis.
    TLine* pline = nullptr;
    // Find the histogram(s) for the plot name.
    if ( splot == "rawv" ) {
      gridx = true;
      gridy = true;
      ph = m_rawHists[icha];
      ph2 = m_vinHists[icha];
      sarg = "e0 x0";
      ph->GetYaxis()->SetTitleOffset(1.3);
      doLine = true;
    }
    else if ( splot == "resp" ) { ph = asa.phc; sarg = "colz"; rightMargin = 0.12; doLine = true; }
    else if ( splot == "zres" ) {
      ph = dynamic_cast<TH2*>(asa.phc->Clone("zres"));
      ph->GetXaxis()->SetRangeUser(0.0, 300.0);
      ph->GetYaxis()->SetRangeUser(-50.0, 150.0);
      sarg = "colz"; rightMargin = 0.12; doLine = true;
      pline = new TLine(0.0, 0.0, 300.0, 0.0);
    }
    else if ( splot == "diff" ) { ph = asa.phd; sarg = "colz"; rightMargin = 0.12; gridy = true; doLine = true; }
    else if ( splot == "difn" ) { ph = asa.phn; sarg = "colz"; rightMargin = 0.12; gridy = true; doLine = true; }
    else if ( splot == "frms" ) { ph = asa.phr; gridy = true; }
    else if ( splot == "fsdv" ) { ph = asa.phs; gridy = true; }
    else if ( splot == "fsdz" ) { ph = dynamic_cast<TH1*>(asa.phs->Clone("fsdz")); ph->SetMaximum(5.0); gridy = true; }
    else if ( splot == "fsdg" ) { ph = asa.phsg; gridy = true; }
    else if ( splot == "fsdb" ) { ph = asa.phsb; gridy = true; }
    else if ( splot == "fsdx" ) { ph = asa.phsx; gridy = true; }
    else if ( splot == "fsdt" ) { ph = asa.pht; logy = true; gridy = true; }
    else if ( splot == "fmea" ) { ph = asa.phm; gridy = true; }
    else if ( splot == "fdr"  ) { ph = asa.phdr; }
    else if ( splot == "fds"  ) { ph = asa.phds; }
    else if ( splot == "fnds" ) { ph = asa.phns; }
    else if ( splot == "fdsb" ) { ph = asa.phdsb; }
    else if ( splot == "veff" ) { ph = asa.phveff; ppad->SetGridx(); gridx = true; gridy = true; }
    else if ( splot == "vrms" ) { ph = asa.phvrms; ppad->SetGridx(); gridx = true; gridy = true; }
    else if ( splot == "vdev" ) { ph = asa.phvdev; ppad->SetGridx(); gridx = true; gridy = true; }
    else if ( splot == "perf" ) {
      ppad->SetGridx();
      asa.drawperf();
      doDraw = false;
    }
    else if ( splot == "gain" ) {
      ph = m_phGain;
      if ( kcha == 0 ) {
        ph->Reset();
        string htitl = sampleName() + " Linear fit gain for " + schanlab;
        ph->SetTitle(htitl.c_str());
      }
      ph->Fill(asa.calib().linearGain());
      doDraw = kcha+1 == nChannel();
    }
    else if ( splot == "offset" ) {
      ph = m_phOffset;
      if ( kcha == 0 ) {
        ph->Reset();
        string htitl = sampleName() + " Linear fit offset for " + schanlab;
        ph->SetTitle(htitl.c_str());
      }
      ph->Fill(asa.calib().linearOffset());
      doDraw = kcha+1 == nChannel();
    }
    else {
      cout << myname << "Invalid plot name: " << splot << endl;
      doDraw = false;
    }
    // If this is a sum, sum instead of drawing.
    if ( doChannelSum ) {
      if ( m_hsums.find(splot) != m_hsums.end() ) {
        TH1*& phs = m_hsums[splot];
        if ( icha == firstChannel() ) {
          delete phs;
          string hname = "hist_" + splot + "_" + namsuf;
          string htitl = ph->GetTitle();
          string::size_type ipos = htitl.find(" channel");
          htitl = htitl.substr(0, ipos+8) + "s " + schan;
          hname += "_sum";
          phs = dynamic_cast<TH1*>(ph->Clone(hname.c_str()));
          phs->SetDirectory(nullptr);
          phs->SetTitle(htitl.c_str());
          m_hists.push_back(phs);
        } else {
          phs->Add(ph);
        }
        doDraw = kcha+1 == nChannel();
        ph = nullptr;
        if ( doDraw ) ph = phs;
      } else {
        cout << myname << "Invalid sum plot name: " << splotin << endl;
        doDraw = false;
      }
    }
    // Gain.
    if ( doLine && pline == nullptr && ph != nullptr ) {
      double xmin = ph->GetXaxis()->GetXmin();
      double xmax = ph->GetXaxis()->GetXmax();
      pline = new TLine(xmin, 0.0, xmax, 0.0);
    }
    if ( doDraw ) {
      ppad->SetRightMargin(rightMargin);
      ppad->SetLeftMargin(leftMargin);
      if ( logy ) ppad->SetLogy();
      if ( gridx ) ppad->SetGridx();
      if ( gridy ) ppad->SetGridy();
      if ( ph == nullptr ) {
        cout << myname << "Histogram for " << splot << " channel " << icha << " not found." << endl;
      } else {
        ph->DrawCopy(sarg.c_str());
        if ( pline != nullptr ) {
          pline->Draw();
          string sargup = sarg + " same";
          ph->DrawCopy(sargup.c_str());
        }
      }
      if ( ph2 != nullptr ) ph2->DrawCopy(sarg2.c_str());
    }
  }
  if ( savePlot ) {
    string fchan;
    if ( nChannel() != 16 ) fchan = "_chan" + schan;
    string fnamesuff = "_" + sampleName() + fchan + ".png";
    string fname = splotin + fnamesuff; 
    pcan->Print(fname.c_str());
  }
  return 0;
}

//**********************************************************************

AdcSampleAnalyzer* AdcChipAnalyzer::sampleAnalyzer(Index icha) {
  const string myname = "AdcChipAnalyzer::sampleAnalyzer: ";
  if ( m_haveAsa.size() < icha+1 ) m_haveAsa.resize(icha+1, false);
  if ( m_asas.size() < icha+1 ) m_asas.resize(icha+1, nullptr);
  if ( m_rawHists.size() < icha+1 ) m_rawHists.resize(icha+1, nullptr);
  if ( m_vinHists.size() < icha+1 ) m_vinHists.resize(icha+1, nullptr);
  AdcSampleAnalyzer*& pasa = m_asas[icha];
  if ( ! m_haveAsa[icha] ) {
    m_haveAsa[icha] = true;
    cout << myname << "Reading data for channel " << icha << endl;
    AdcSampleFinder asf;
    AdcSampleFinder::AdcSampleReaderPtr prdr = asf.find(sampleName(), icha);
    if ( ! prdr ) {
      cout << myname << "Sample " << sampleName() << " not found for channel "
           << icha << "." << endl;
    }
    AdcChannelCalibration* pcal = nullptr;
    if ( datasetCalib().size() ) {
      AdcCalibrationTree act(datasetCalib());
      Index ient = 0;
      const AdcTreeChannelCalibration* pcaltmp = act.find(prdr->chip(), icha, ient);
      if ( pcaltmp == nullptr ) {
        cout << myname << "WARNING: Unable to find calibration " << datasetCalib()
             << " for channel " << icha << ". Skipping channel." << endl;
        return nullptr;
      }
      pcal = new AdcTreeChannelCalibration(*pcaltmp);
    }
    m_pcals.push_back(pcal);    // Record this for deletion in dtor.
    pasa = new AdcSampleAnalyzer(std::move(prdr), pcal, fixped);
    AdcSampleAnalyzer& asa = *pasa;
    if ( asa.phc != nullptr ) {
      // Create array of voltage responses.
      asa.evaluateVoltageResponses(vmin(), vmax(), nv());
      // Create efficiency histogram.
      asa.evaluateVoltageEfficiencies(vRmsMax(), true, dropTails());
      // Add calibration to tree.
      if ( saveCalib() ) {
        if ( asa.calib().isValid() && asa.dataset().size() ) {
          cout << myname << "Adding channel " << asa.calib().channel() << " to calib DB." << endl;
          string calibName = "calib_" + asa.dataset();
          string fname = calibName + ".root";
          AdcCalibrationTree calibdb(fname, "adccalib", "UPDATE");
          int istat = calibdb.insert(asa.localCalib());
          cout << myname << "Insertion returned " << istat << endl;
        } else {
          cout << myname << "Not adding channel " << asa.channel() << " to calib DB." << endl;
        }
      }
      // Add performance to tree.
      if ( savePerf() ) {
        if ( asa.vperfs.size() && asa.dataset().size() ) {
          cout << myname << "Adding channel " << asa.channel() << " to performance DB." << endl;
          string perfName = "perf_" + asa.dataset();
          string fname = perfName + ".root";
          AdcPerformanceTree perfdb(fname, "adcperf", "UPDATE");
          for ( const AdcVoltagePerformance& vperf : asa.vperfs ) {
            unsigned int oldsize = perfdb.size();
            int istat = perfdb.insert(vperf);
            unsigned int newsize = perfdb.size();
            cout << myname << "Insertion returned " << istat << ".";
            cout << " Old-->new size = " << oldsize << "-->" << newsize << "." << endl;
          }
        } else {
          cout << "Not adding channel " << asa.channel() << " to perf DB." << endl;
        }
      }
    }
    // Fetch the waveform histograms.
    const AdcSampleReader& rdr = *asa.reader();
    int rebin = 1000;
    if ( rdr.nsample() < 1000000 ) rebin = 100;
    if ( rdr.nsample() < 100000 ) rebin = 10;
    if ( rdr.nsample() < 10000 ) rebin = 1;
    TH1* ph = rdr.histdata(0, 0, -rebin, true);
    ph->SetLineWidth(2);
    ph->SetMinimum(-500);
    ph->SetMaximum(4500);
    TH1* ph2 = rdr.histvin(0, 0, rebin, true);
    ph2->SetLineWidth(2);
    ph2->SetLineColor(2);
    string ylab = "ADC code, Input voltage [mV]";
    if ( ph2 != nullptr ) ph->GetYaxis()->SetTitle(ylab.c_str());
    m_rawHists[icha] = ph;
    m_vinHists[icha] = ph2;
    // If flag is set, clean the analyzer.
    // This deletes some less-critical histograms and deletes the reader.
    if ( clean() ) {
      pasa->clean();
      //gDirectory->DeleteAll();   // Delete all the histograms to make room for the next channel.
    }
    if ( true ) {
      ProcInfo_t info;
      gSystem->GetProcInfo(&info);
      cout << myname << "Virtual memory after channel " << setw(2) << asa.channel() << " is "
           << info.fMemVirtual/1000000.0 << " GB" << endl;
    }
  }
  if ( pasa == nullptr || pasa->phc == nullptr ) {
    cout << myname << "ERROR: ADC analysis failed for channel " << icha << "." << endl;
  }
  return pasa;
}

//**********************************************************************

Index AdcChipAnalyzer::channel(Index kcha) const {
  if ( kcha >= nChannel() ) return nChannel();
  return m_chans[kcha];
}

//**********************************************************************
