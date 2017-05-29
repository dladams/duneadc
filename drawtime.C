// drawtime.C

#include "AdcTimeAnalyzer.h"
#include <iostream>
#include <vector>
#include <iomanip>
#include "TH1.h"
#include "TCanvas.h"

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::setprecision;
using std::fixed;

bool keep(TH1* phcount, TH1* phrms) {
  double cntMean = phcount->GetMean();
  if ( cntMean < 200.0 ) return false;
  double resRms = phrms->GetMean();
  if ( resRms > 0.90 && resRms < 1.1 ) return false;
  return true;
}

int drawChannelTime(Index icha =1, Index* pnkeepTot =nullptr, Index* pnskipTot =nullptr) {
  const string myname = "drawChannelTime: ";
  string dsname = "201703b_0516";
  Index chip = 2;
  AdcTimeAnalyzer ata("201703b_0516", chip, icha);
  ata.report();
  Index kbin = 0;
  Index kbinLast = kbin;
  TCanvas* pcan = nullptr;
  vector<TCanvas*> pcans;
  vector<string> rowvar = {"RmsTime", "CountTime"};
  //vector<string> rowvar = {"RmsTime", "Residual", "CountTime"};
  Index ncol = rowvar.size();
  Index nrow = ncol;
  Index nkeep = 0;
  Index nskip = 0;
  Index icol = 0;
  for ( Index ibin : ata.gbvaryBins ) {
    TH1* phcDist = ata.histCount(ibin);
    TH1* phrDist = ata.histRms(ibin);
    if ( keep(phcDist, phrDist) ) {
      cout << myname << "Keeping bin " << ibin << endl;
      icol = icol%ncol;
      if ( icol == 0 ) {
        pcan = new TCanvas;
        pcan->Divide(ncol, nrow);
        pcans.push_back(pcan);
      }
      TH1* ph = nullptr;
      for ( Index irow=0; irow<nrow; ++irow ) {
        string svar = rowvar.at(irow);
        TH1* ph = nullptr;
        bool logy = false;
        if ( svar == "CountTime" ) {
          ph = ata.histCountTime(ibin);
          ph->SetMinimum(1.0);
          ph->SetMaximum(5.0e4);
          logy = true;
        } else if ( svar == "Mean" ) {
          ph = ata.histMean(ibin, 0, 0);
        } else if ( svar == "Residual" ) {
          ph = ata.histMean(ibin, 0, 1);
        } else if ( svar == "PedMean" ) {
          ph = ata.histMean(ibin, 0, 2);
        } else if ( svar == "Rms" ) {
          ph = ata.histRms(ibin);
        } else if ( svar == "RmsTime" ) {
          ph = ata.histRmsTime(ibin);
        } else {
          cout << myname << "Invalid row variable name: " << svar << endl;
          return 1;
        }
        TVirtualPad* ppad = pcan->cd(ncol*irow + icol + 1);
        ppad->SetGridy();
        ppad->SetRightMargin(0.02);
        if ( logy ) ppad->SetLogy();
        ph->Draw();
      }
      ++icol;
      ++nkeep;
    } else  {
      cout << myname << "Skipping bin " << ibin << endl;
      ++nskip;
    }
  }
  double den = ata.iadc2 - ata.iadc1;
  cout << myname << "Channel " << icha << ": nkeep=" << nkeep << ", nskip=" << nskip;
  cout << ", fkeep = "
       << setprecision(1) << fixed << 100.0*nkeep/den << "%"
       << " = "
       << setprecision(2) << fixed << 100.0*nkeep/den << "%" << endl;
  cout << endl;
  if ( pnkeepTot != nullptr ) (*pnkeepTot) += nkeep;
  if ( pnskipTot != nullptr ) (*pnskipTot) += nskip;
  for ( Index ican=0; ican<pcans.size(); ++ican ) {
    ostringstream ssfname;
    ssfname << "timevary_" << dsname << "_chip" << chip << "_chan";
    if ( icha < 10 ) ssfname << "0";
    ssfname << icha << "_page";
    if ( ican < 10 ) ssfname << "0";
    ssfname << ican << ".png";
    string fname = ssfname.str();
    pcans[ican]->Print(fname.c_str());
  }
  return 0;
}

int drawtime(Index icha1, Index ncha =1) {
  const string myname = "drawtime: ";
  Index icha2 = icha1 + ncha;
  Index nkeep = 0;
  Index nskip = 0;
  for ( Index icha=icha1; icha<icha2; ++icha ) {
    drawChannelTime(icha, &nkeep, &nskip);
  }
  double den = ncha*4094;
  if ( ncha > 1 ) {
    cout << myname << "Channels " << icha1 << "-" << icha2-1 << ": nkeep=" << nkeep << ", nskip=" << nskip << endl;
    cout << myname << "  fkeep = "
         << setprecision(1) << fixed << 100.0*nkeep/den << "%"
         << " = "
         << setprecision(2) << fixed << 100.0*nkeep/den << "%" << endl;
    cout << myname << "  fvary="
         << setprecision(1) << fixed << 100.0*(nkeep+nskip)/den << "%" << endl;
  }
  return 0;
}
