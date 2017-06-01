// AdcDatasetAnalyzer.cxx

#include "AdcDatasetAnalyzer.h"
#include <memory>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "AdcTreeChannelCalibration.h"
#include "TH2F.h"
#include "TCanvas.h"
#include "TPaletteAxis.h"
#include "TList.h"

using std::cout;
using std::endl;
using std::ostringstream;
using std::setw;
using Index = AdcDatasetAnalyzer::Index;
using IndexVector = AdcDatasetAnalyzer::IndexVector;
using NameVector = AdcDatasetAnalyzer::NameVector;
using AdcTreeChannelCalibrationPtr = const AdcTreeChannelCalibration*;

//**********************************************************************

AdcDatasetAnalyzer::
AdcDatasetAnalyzer(Index achip, Index aibin1, Index aibin2)
: AdcDatasetAnalyzer(NameVector(), achip, aibin1, aibin2) { }

//**********************************************************************

AdcDatasetAnalyzer::
AdcDatasetAnalyzer(const NameVector& adsts, Index achip, Index aibin1, Index aibin2)
: dsts(adsts), chip(achip), ibin1(aibin1), ibin2(aibin2) {
  const string myname = "AdcDatasetAnalyzer::ctor: ";
  if ( dsts.size() == 0 ) {
    dsts.push_back("201701c");
    dsts.push_back("20170216");
    dsts.push_back("20170217");
    dsts.push_back("20170218");
    dsts.push_back("20170219");
    dsts.push_back("20170220");
    dsts.push_back("20170221");
    dsts.push_back("20170222");
    dsts.push_back("20170223");
  }
  unsigned int maxchan = 16;
  hres.resize(maxchan, nullptr);
  hresj.resize(maxchan, nullptr);
  hcnt.resize(maxchan, nullptr);
  hcntj.resize(maxchan, nullptr);
  chanNLowStat.resize(maxchan, 0);
  chanNAlwaysBad.resize(maxchan, 0);
  chanNAlwaysGood.resize(maxchan, 0);
  chanNJump.resize(maxchan, 0);
  chanNElse.resize(maxchan, 0);
}

//**********************************************************************

int AdcDatasetAnalyzer::fill(Index chan) {
  const Name myname = "AdcDatasetAnalyzer::fill: ";
  if ( chan > hcnt.size() ) return 2;
  if ( hcnt[chan] != nullptr ) return 0;
  vector<AdcTreeChannelCalibrationPtr> cals(dsts.size());
  for ( Index idst=0; idst<dsts.size(); ++idst ) {
    Name dst = dsts[idst];
    AdcTreeChannelCalibrationPtr pcal(AdcTreeChannelCalibration::find(dst, chip, chan));
    if ( pcal == nullptr ) {
      cout << myname << "Unable to find calibration for " << dst << " chip " << chip << " channel " << chan << endl;
      return 3;
    }
    cals[idst] = std::move(pcal);
  }
  unsigned int ndst = dsts.size();
  ostringstream sshname;
  ostringstream sshtitl;
  sshname << "hres_" << chip << "_" << chan << "_" << ibin1 << "_" << ibin2;
  sshtitl << "ADC bin resolution for all bins: chip " << chip << " channel " << chan;
  sshtitl << ";ADC bin; Dataset; Resolution [mV]";
  string hname = sshname.str();
  string htitl = sshtitl.str();
  TH2* phr = new TH2F(hname.c_str(), htitl.c_str(), 4100, 0, 4100, ndst, 0, ndst);
  hres[chan] = phr;
  phr->SetContour(20);
  phr->SetStats(0);
  phr->SetMinimum(0.0);
  phr->SetMaximum(1.2);
  sshname.str("");
  sshtitl.str("");
  sshname << "hcnt_" << chip << "_" << chan << "_" << ibin1 << "_" << ibin2;
  sshtitl << "ADC bin count for all bins: chip " << chip << " channel " << chan;
  sshtitl << ";ADC bin; Dataset; log_{10}(# samples)";
  string hnamec = sshname.str();
  string htitlc = sshtitl.str();
  TH2* phc = new TH2F(hnamec.c_str(), htitlc.c_str(), 4100, 0, 4100, ndst, 0, ndst);
  hcnt[chan] = phc;
  phc->SetContour(20);
  phc->SetStats(0);
  phc->SetMinimum(0.0);
  phc->SetMaximum(5);
  phc->GetYaxis()->SetLabelSize(0.05);
  phr->GetYaxis()->SetLabelSize(0.05);
  for ( unsigned int idst=0; idst<ndst; ++idst ) {
    phc->GetYaxis()->SetBinLabel(idst+1, dsts[idst].c_str());
    phr->GetYaxis()->SetBinLabel(idst+1, dsts[idst].c_str());
  }
  vector<unsigned int> jumpbins;
  for ( unsigned int ibin=ibin1; ibin<ibin2; ++ibin ) {
    float resLast = -1.0;
    bool jump = false;
    unsigned int ngood = 0;
    unsigned int nbad = 0;
    for ( unsigned int idst=0; idst<ndst; ++idst ) {
      unsigned int bin = phr->GetBin(ibin+1, idst+1);
      const AdcTreeChannelCalibrationPtr& pcal = cals[idst];
      unsigned short cnt = pcal->calCount(ibin);
      float res = pcal->calRms(ibin);
      if ( cnt > 0 ) phc->SetBinContent(bin, log10(cnt));
      if ( res > 0.0 && cnt > 100 ) {     // res < 0 is insufficient stats
        phr->SetBinContent(bin, res);
        if ( resLast > 0.0 ) {
          if ( fabs(res-resLast) > 0.1 ) jump = true;
        }
        resLast = res;
        if ( res < 1.0 ) ++ngood;
        else ++nbad;
      }
    }
    // Record bins that have a jump and are sometimes good.
    if ( ngood && nbad ) {
      jumpbins.push_back(ibin);
      ++chanNJump[chan];
    } else if ( ngood == 0 && nbad == 0 ) {
      ++chanNLowStat[chan];
    } else if ( ngood == 0 ) {
      ++chanNAlwaysBad[chan];
    } else if ( nbad == 0 ) {
      ++chanNAlwaysGood[chan];
    } else {
      ++chanNElse[chan];
    }
  }
  unsigned int njmp = jumpbins.size();
  cout << "Jump bin count: " << njmp << endl;
  TH1* ph = phr;
  if ( njmp ) {
    hname.replace(0, 4, "hresj");
    string::size_type ipos = htitl.find("all");
    htitl.replace(ipos, 3, "jumping");
    TH2* phj = new TH2F(hname.c_str(), htitl.c_str(), njmp, 0, njmp, ndst, 0, ndst);
    hnamec.replace(0, 4, "hresj");
    ipos = htitlc.find("all");
    htitlc.replace(ipos, 3, "jumping");
    TH2* phcj = new TH2F(hnamec.c_str(), htitlc.c_str(), njmp, 0, njmp, ndst, 0, ndst);
    hresj[chan] = phj;
    hcntj[chan] = phcj;
    phj->GetXaxis()->SetLabelSize(0.05);
    phcj->GetXaxis()->SetLabelSize(0.05);
    for ( unsigned int ijmp=0; ijmp<njmp; ++ijmp ) {
      ostringstream ssbin;
      ssbin << jumpbins[ijmp];
      string sbin = ssbin.str();
      phj->GetXaxis()->SetBinLabel(ijmp+1, sbin.c_str());
      phcj->GetXaxis()->SetBinLabel(ijmp+1, sbin.c_str());
    }
    if ( njmp > 40 ) phj->GetXaxis()->LabelsOption("v");
    phj->GetYaxis()->SetLabelSize(0.05);
    phcj->GetYaxis()->SetLabelSize(0.05);
    //phj->GetYaxis()->SetTitle("");
    for ( unsigned int idst=0; idst<ndst; ++idst ) {
      phj->GetYaxis()->SetBinLabel(idst+1, dsts[idst].c_str());
      phcj->GetYaxis()->SetBinLabel(idst+1, dsts[idst].c_str());
    }
    phj->SetContour(20);
    phj->SetStats(0);
    phj->SetMinimum(0.0);
    phj->SetMaximum(1.2);
    phcj->SetContour(20);
    phcj->SetStats(0);
    phcj->SetMinimum(0.0);
    phcj->SetMaximum(5);
    for ( unsigned int ijmp=0; ijmp<njmp; ++ijmp ) {
      unsigned int ibin = jumpbins[ijmp];
      if ( dbg ) cout << myname << "Bin " << ibin << endl;
      for ( unsigned int idst=0; idst<ndst; ++idst ) {
        const AdcTreeChannelCalibrationPtr& pcal = cals[idst];
        unsigned int bin = phj->GetBin(ijmp+1, idst+1);
        float res = pcal->calRms(ibin);
        unsigned short cnt = pcal->calCount(ibin);
        if ( cnt > 0 ) phcj->SetBinContent(bin, log10(cnt));
        if ( dbg ) cout << myname << "  Count " << pcal->calCount(ibin) << endl;
        if ( res > 0.0 ) {
          phj->SetBinContent(bin, res);
        }
      }
    }
    ph = phj;
  }
  return 0;
}

//**********************************************************************

void AdcDatasetAnalyzer::draw(Name name, Index chan) {
  if ( fill(chan) != 0 ) return;
  TH1* ph = nullptr;
  if ( name == "res" )  ph = hres[chan];
  if ( name == "resj" ) ph = hresj[chan];
  if ( name == "cnt" )  ph = hcnt[chan];
  if ( name == "cntj" ) ph = hcntj[chan];
  if ( ph == nullptr ) return;
  ostringstream sscname;
  sscname << name << "dst_" << chip << "_" << chan;
  string cname = sscname.str();
  TCanvas* pcan = new TCanvas(cname.c_str(), cname.c_str(), 1500, 500);
  pcan->SetRightMargin(0.06);
  pcan->SetLeftMargin(0.085);
  pcan->SetBottomMargin(0.15);
  ph->SetTitleOffset(0.7, "y");
  ph->GetXaxis()->SetTitleOffset(2.0);
  ph->GetYaxis()->SetTickLength(0.01);
  ph->GetZaxis()->SetTickLength(0.01);
  ph->GetYaxis()->SetTitleOffset(1.3);
  ph->GetZaxis()->SetTitleOffset(0.55);
  ph->Draw("colz");
  double palx1 = 0.941;
  double palx2 = 0.960;
  gPad->Update();
  TPaletteAxis* ppalax = dynamic_cast<TPaletteAxis*>(ph->GetListOfFunctions()->FindObject("palette"));
  if ( ppalax != nullptr ) {
    ppalax->SetX1NDC(palx1);
    ppalax->SetX2NDC(palx2);
  }
  ph->DrawCopy("colz");
}

//**********************************************************************

void AdcDatasetAnalyzer::draw(Name name, Index chan1, Index chan2) {
  if ( chan2 <= chan1 ) return draw(name, chan1);
  for ( Index chan=chan1; chan<chan2; ++chan ) draw(name, chan);
}

//**********************************************************************

void AdcDatasetAnalyzer::report(Index chan) const {
  const Name myname = "AdcDatasetAnalyzer::report: ";
  // Print report.
  unsigned int ntot = chanNLowStat[chan] + chanNAlwaysBad[chan] + chanNAlwaysGood[chan] + chanNJump[chan] + chanNElse[chan];
  cout << myname << "Chip " << chip << " channel " << chan << endl;
  cout << myname << "    # low stat:" << setw(5) << chanNLowStat[chan] << endl;
  cout << myname << "  # always bad:" << setw(5) << chanNAlwaysBad[chan] << endl;
  cout << myname << " # always good:" << setw(5) << chanNAlwaysGood[chan] << endl;
  cout << myname << "       # jumps:" << setw(5) << chanNJump[chan] << endl;
  cout << myname << "        # else:" << setw(5) << chanNElse[chan] << endl;
  cout << myname << "           SUM:" << setw(5) << ntot << endl;
}

//**********************************************************************

void AdcDatasetAnalyzer::report(Name prefix) {
  //const Name myname = "AdcDatasetAnalyzer::report: ";
  Index w = 7;
  Index maxchan = 16;
  for ( Index chan=0; chan<maxchan; ++chan ) fill(chan);
  cout << prefix << "Chip " << chip
       << " (" << ibin1 << " <= bin < " << ibin2 << ")" << endl;
  cout << prefix << setw(w) << "chan" << ": "
       << setw(w) << "lostat"
       << setw(w) << "jump"
       << setw(w) << "bad"
       << setw(w) << "good"
       << setw(w) << "else"
       << setw(w) << "SUM" << endl;
  for ( Index chan=0; chan<maxchan; ++chan ) {
    unsigned int ntot = chanNLowStat[chan] + chanNAlwaysBad[chan] + chanNAlwaysGood[chan] + chanNJump[chan] + chanNElse[chan];
    if ( ntot > 0 ) {
      cout << prefix << setw(w) << chan << ": "
           << setw(w) << chanNLowStat[chan]
           << setw(w) << chanNJump[chan]
           << setw(w) << chanNAlwaysBad[chan]
           << setw(w) << chanNAlwaysGood[chan]
           << setw(w) << chanNElse[chan]
           << setw(w) << ntot << endl;
    }
  }
}

//**********************************************************************
