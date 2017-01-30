// makeplot.cxx

#include "makeplot.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include "AdcHist.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TH1F.h"
#include "TLine.h"
#include "TStyle.h"

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;
using std::vector;
using std::map;
typedef unsigned int Index;

void makeplot(string ssam, Index icha1, Index ncha) {
  string myname = "responseplots: ";
  gStyle->SetOptStat(110111);
  vector<string> stypes = {"resp", "diff", "difn", "zres", "frms", "fsdv", "fsdz", "fsdg", "fmea", "fdn", "fdr", "fds", "fdsb"};
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
  string hgtitl = ssam + " fit Vin/ADC; Gain [mV/ADC]; # channels";
  TH1* phg = new TH1F("hg", hgtitl.c_str(), 60, 0, 0.6);
  string hptitl = ssam + " fit offset; Offset [mV]; # channels";
  TH1* php = new TH1F("hp", hptitl.c_str(), 40, -100, 100);
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
    AdcHist adchist(ssam, icha);
    if ( adchist.phc == nullptr ) {
      cout << "No response plot for " << ssam << " channel " << icha << endl;
      return;
      continue;
    }
    for ( unsigned int ityp=0; ityp<stypes.size(); ++ityp ) {
      string stype = stypes[ityp];
      TCanvas* pcan = cans[ityp];
      TVirtualPad* ppad = pcan->cd(ipad);
      ppad->SetGridy();
      TH1* ph = nullptr;
      if ( stype == "resp" ) ph = adchist.phc;
      if ( stype == "diff" ) ph = adchist.phd;
      if ( stype == "difn" ) ph = adchist.phn;
      if ( stype == "frms" ) ph = adchist.phr;
      if ( stype == "fsdv" ) ph = adchist.phs;
      if ( stype == "fsdg" ) ph = adchist.phsg;
      if ( stype == "fsdz" ) {
        ph = dynamic_cast<TH1*>(adchist.phs->Clone("fsdv"));
        ph->SetMaximum(5.0);
      }
      if ( stype == "fmea" ) ph = adchist.phm;
      if ( stype == "fdn" ) ph = adchist.phdn;
      if ( stype == "fdr" ) ph = adchist.phdr;
      if ( stype == "fds" ) ph = adchist.phds;
      if ( stype == "fdsb" ) ph = adchist.phdsb;
      if ( stype == "zres" ) {
        ph = dynamic_cast<TH2*>(adchist.phc->Clone("zres"));
        ph->GetXaxis()->SetRangeUser(0.0, 300.0);
        ph->GetYaxis()->SetRangeUser(-50.0, 150.0);
      }
      if ( ph == nullptr ) continue;
      if ( stype == "diff" ||
           stype == "difn" ||
           stype == "fmea" ) {
        double xmin = ph->GetXaxis()->GetXmin();
        double xmax = ph->GetXaxis()->GetXmax();
        ph->Draw("colz");
        TLine* pline = new TLine(xmin, 0.0, xmax, 0.0);
        pline->Draw();
        ph->Draw("colz same");
      } else {
        ph->Draw("colz");
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
          phs->SetTitle(htitl.c_str());
          fhists.push_back(phs);
          string slab = stype + "_sum";
          flabs.push_back(slab);
        } else {
          phs->Add(ph);
        }
      }
    }
    phg->Fill(adchist.fitVinPerAdc);
    php->Fill(adchist.fitped);
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
    ph->Draw();
    string fname = flabs[ihst] + fnamesuff; 
    pcan->Print(fname.c_str());
  }
}

