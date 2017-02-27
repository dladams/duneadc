// AdcChipAnalyzer.h

#include "AdcChipAnalyzer.h"
#include "AdcCalibrationTree.h"
#include "AdcPerformanceTree.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "AdcSampleAnalyzer.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TLine.h"
#include "TStyle.h"
#include "TLegend.h"

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;
using std::vector;
using std::map;
typedef unsigned int Index;

AdcChipAnalyzer::
AdcChipAnalyzer(string ssam, Index icha1, Index ncha, bool savecalib,
                float vmin, float vmax, Index nv, double vrmsmax, bool saveperf) {
  string myname = "AdcChipAnalyzer::ctor: ";
  gStyle->SetOptStat(110111);
  vector<string> stypes = {"resp", "diff", "difn", "zres", "frms", "fsdv", "fsdz", "fsdg", "fmea", "fdn", "fdr", "fds", "fdsb", "veff"};
  //vector<string> stypes = {"fdn", "fdr", "fds", "fdsb"};
  Index maxchan = 16;
  if ( ssam.substr(0,6) == "201610" ) maxchan = 15;
  if ( ncha == 0 ) ncha = maxchan - icha1;
  // Assign the channel numbers to include.
  vector<Index> chans;
  vector<Index> pads;
  if ( ssam == "201612_ltc2314" && ncha > 0 ) {
    ncha = 4;
    chans.push_back(0);
    chans.push_back(1);
    chans.push_back(8);
    chans.push_back(11);
    pads.push_back(1);
    pads.push_back(2);
    pads.push_back(3);
    pads.push_back(4);
  } else {
    for ( Index icha=icha1; icha<icha1+ncha; ++icha ) chans.push_back(icha);
    for ( Index icha=icha1; icha<icha1+ncha; ++icha ) pads.push_back(icha+1);
  }
  // Evaluate the layout of plots on a page (1, 4 or 16).
  Index npadx = 0;
  if ( ncha > 4 ) {
    npadx = 4;
  } else if ( ncha > 1 ) {
    npadx = 2;
  }
  Index npady = npadx;
  Index npad = npadx*npadx;
  Index wwx = 700;
  Index wwy = 500;
  if ( gROOT->IsBatch() && npad ) {
    wwx *= npadx;
    wwy *= npady;
  }
  vector<TCanvas*> cans;
  for ( string stype : stypes ) {
    TCanvas* pcan = new TCanvas(stype.c_str(), stype.c_str(), wwx, wwy);
    if ( npadx ) pcan->Divide(npadx, npadx);
    cans.push_back(pcan);
  }
  // Create linear-fit gain summary histogram.
  string hgtitl = ssam + " fit Vin/ADC; Gain [mV/ADC]; # channels";
  TH1* phg = new TH1F("hg", hgtitl.c_str(), 60, 0, 0.6);
  phg->SetDirectory(0);   // Don't delete this
  // Create linear-fit offset summary histogram.
  string hptitl = ssam + " fit offset; Offset [mV]; # channels";
  TH1* php = new TH1F("hp", hptitl.c_str(), 40, -100, 100);
  php->SetDirectory(0);   // Don't delete this
  // Record local histograms.
  vector<TH1*> fhists = {phg, php};
  vector<string> flabs = {"fitgain", "fitped"};
  map<string, TH1*> hsums;  // Channels will be summed for these types.
  hsums["fdn"] = nullptr;
  hsums["fdr"] = nullptr;
  hsums["fds"] = nullptr;
  hsums["fdsb"] = nullptr;
  for ( Index kcha=0; kcha<ncha; ++kcha ) {
    Index icha = chans[kcha];
    Index ipad = npad ? pads[kcha] : 0;
    if ( npad ) cans[0]->cd(ipad);
    AdcSampleAnalyzer asa(ssam, icha);
    // Create array of voltage responses.
    asa.evaluateVoltageResponses(vmin, vmax, nv);
    // Create efficiency histogram.
    asa.evaluateVoltageEfficiencies(vrmsmax);
    // Create histogram plots.
    if ( asa.phc == nullptr ) {
      cout << "No response plot for " << ssam << " channel " << icha << endl;
      return;
      continue;
    }
    for ( unsigned int ityp=0; ityp<stypes.size(); ++ityp ) {
      bool addvrms = false;
      string stype = stypes[ityp];
      TCanvas* pcan = cans[ityp];
      TVirtualPad* ppad = pcan->cd(ipad);
      ppad->SetGridy();
      TH1* ph = nullptr;
      string sarg = "colz";
      if ( stype == "resp" ) ph = asa.phc;
      if ( stype == "diff" ) ph = asa.phd;
      if ( stype == "difn" ) ph = asa.phn;
      if ( stype == "frms" ) ph = asa.phr;
      if ( stype == "fsdv" ) ph = asa.phs;
      if ( stype == "fsdg" ) ph = asa.phsg;
      if ( stype == "fsdz" ) {
        ph = dynamic_cast<TH1*>(asa.phs->Clone("fsdv"));
        ph->SetMaximum(5.0);
      }
      if ( stype == "fmea" ) ph = asa.phm;
      if ( stype == "fdn"  ) ph = asa.phdn;
      if ( stype == "fdr"  ) ph = asa.phdr;
      if ( stype == "fds"  ) ph = asa.phds;
      if ( stype == "fdsb" ) ph = asa.phdsb;
      if ( stype == "zres" ) {
        ph = dynamic_cast<TH2*>(asa.phc->Clone("zres"));
        ph->GetXaxis()->SetRangeUser(0.0, 300.0);
        ph->GetYaxis()->SetRangeUser(-50.0, 150.0);
      }
      if ( stype == "veff" ) {
        ph = asa.phveff;
        ppad->SetGridx();
        sarg = "h";
        addvrms = true;
      }
      if ( ph == nullptr ) continue;
      if ( dynamic_cast<TH2*>(ph) == nullptr ) ppad->SetRightMargin(0.04);
      if ( stype == "diff" ||
           stype == "difn" ||
           stype == "fmea" ) {
        double xmin = ph->GetXaxis()->GetXmin();
        double xmax = ph->GetXaxis()->GetXmax();
        ph->DrawCopy(sarg.c_str());
        TLine* pline = new TLine(xmin, 0.0, xmax, 0.0);
        pline->Draw();
        string sarg2 = sarg + " same";
        ph->DrawCopy(sarg2.c_str());
      } else if ( addvrms ) {
        string hnam = ph->GetName();
        hnam += "_vrms";
        TH1* phax = dynamic_cast<TH1*>(ph->Clone(hnam.c_str()));
        string ylab = phax->GetYaxis()->GetTitle();
        ylab += ", V_{in} resolution [mV]";
        phax->GetYaxis()->SetTitle(ylab.c_str());
        //phax->SetLineColor(kBlue+1);
        //phax->SetLineWidth(2);
        phax->SetDirectory(0);   // Leaking this preserves the line color/style in the legend
        phax->DrawCopy("h");
        asa.pgvrms->Draw("Z");
        TLegend* pleg = new TLegend(0.5, 0.15, 0.65, 0.25);
        pleg->SetBorderSize(0);
        pleg->SetFillStyle(0);
        pleg->AddEntry(phax, "Efficiency", "l");
        pleg->AddEntry(asa.pgvrms, "Resolution", "le");
        pleg->Draw();
      } else {
        ph->DrawCopy(sarg.c_str());
      }
      if ( hsums.find(stype) != hsums.end() ) {
        TH1*& phs = hsums[stype];
        if ( phs == nullptr ) {
          string hname = ph->GetName();
          string htitl = ph->GetTitle();
          string::size_type ipos = htitl.find(" channel");
          htitl = htitl.substr(0, ipos);
          hname += "_sum";
          phs = dynamic_cast<TH1*>(ph->Clone(hname.c_str()));
          phs->SetDirectory(nullptr);
          phs->SetTitle(htitl.c_str());
          fhists.push_back(phs);
          string slab = stype + "_sum";
          flabs.push_back(slab);
        } else {
          phs->Add(ph);
        }
      }
    }
    phg->Fill(asa.fitVinPerAdc);
    php->Fill(asa.fitped);
    if ( savecalib ) {
      if ( asa.calib.isValid() && asa.reader.dataset().size() ) {
        cout << "Adding channel " << asa.calib.chan << " to calib DB." << endl;
        string calibName = "calib_" + asa.reader.dataset();
        string fname = calibName + ".root";
        AdcCalibrationTree calibdb(fname, "adccalib", "UPDATE");
        int istat = calibdb.insert(asa.calib);
        cout << "Insertion returned " << istat << endl;
      } else {
        cout << "Not adding channel " << asa.calib.chan << " to calib DB." << endl;
      }
    }
    if ( saveperf ) {
      if ( asa.vperfs.size() && asa.reader.dataset().size() ) {
        cout << "Adding channel " << asa.channel() << " to calib DB." << endl;
        string perfName = "perf_" + asa.reader.dataset();
        string fname = perfName + ".root";
        AdcPerformanceTree perfdb(fname, "adcperf", "UPDATE");
        for ( const AdcVoltagePerformance& vperf : asa.vperfs ) {
          int istat = perfdb.insert(vperf);
          cout << "Insertion returned " << istat << endl;
        }
      } else {
        cout << "Not adding channel " << asa.channel() << " to perf DB." << endl;
      }
    }
    gDirectory->DeleteAll();   // Delete all the histograms to make room for the next channel.
  }
  string schan;
  if ( npad < 16 ) {
    ostringstream sschan;
    sschan << "_chan";
    if ( icha1 < 10 ) sschan << "0";
    sschan << icha1;
    if ( ncha > 1 ) {
      sschan << "_n" << ncha;
    }
    schan = sschan.str();
  }
  string fnamesuff = "_" + ssam + schan + ".png";
  for ( Index ityp=0; ityp<stypes.size(); ++ityp ) {
    string stype = stypes[ityp];
    TCanvas* pcan = cans[ityp];
    string fname = stype + fnamesuff; 
    pcan->Print(fname.c_str());
  }
  for ( Index ihst=0; ihst<fhists.size(); ++ihst ) {
    TCanvas* pcan = new TCanvas;
    TH1* ph = fhists[ihst];
    ph->SetLineWidth(2);
    ph->DrawCopy();
    string fname = flabs[ihst] + fnamesuff; 
    pcan->Print(fname.c_str());
    delete ph;
  }
}

