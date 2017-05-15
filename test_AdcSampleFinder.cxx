// test_AdcSampleFinder.cxx

#include "AdcSampleFinder.h"
#include "ErrorCount.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include <iostream>
#include <sstream>

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;

namespace {

string line() {
  return "-----------------------------------------";
}

void addLegend(TH1* phAdc, TH1* phVin) {
  TLegend* pleg = new TLegend(0.18, 0.77, 0.33, 0.87);
  pleg->SetBorderSize(0);
  pleg->SetFillStyle(0);
  pleg->AddEntry(phAdc, "ADC", "l");
  pleg->AddEntry(phVin, "V_{in}", "l");
  pleg->Draw();
  double x1 = phAdc->GetXaxis()->GetXmin();
  double x2 = phAdc->GetXaxis()->GetXmax();
  TLine* pline = new TLine(x1, 0.0, x2, 0.0);
  pline->Draw();
}

int test_201703a(Index icha) {
  const string myname = "test_201703a: ";
  cout << line() << endl;
  string dsname = "201703a_D22";
  cout << myname << "Testing " << dsname << endl;
  cout << line() << endl;
  ErrorCount ec(myname + "Test failed: ");
  AdcSampleFinder asf;
  AdcSampleFinder::AdcSampleReaderPtr prdr = asf.find(dsname, icha);
  ec.check( prdr->dataset() == "201703a", "Dataset name" );
  ec.check( prdr->chip() == 22, "Chip index" );
  ec.check( prdr->chipLabel() == "D22", "Chip label" );
  ec.check( prdr->channel() == icha, "Channel" );
  ec.check( prdr->nchannel() == 16, "# channel" );
  ec.check( prdr->nadc() == 4096, "# ADC bins" );
  ec.check( prdr->maxSample() == 0, "Max # samples" );
  ec.check( prdr->nsample() == 9000000, "# samples" );
  vector<SampleValue> expCode6M = {    0,    0,    0,    0,
                                    3131, 3124,    0,    0,
                                       0,    0,    0,    0,
                                       0,    0,    0,    0};
  ec.checkequal( prdr->code(6000000), expCode6M[icha], "code(6000000)" );
  vector<int> expvin0M(16, -200);
  vector<int> expvin6M(16, 1000);
  vector<int> expvin9M(16, 1600);
  ec.checkequal( int(prdr->vin(      0)), expvin0M[icha], "vin(0)" );
  ec.checkequal( int(prdr->vin(6000000)), expvin6M[icha], "vin(6000000)" );
  ec.checkequal( int(prdr->vin(9000000)), expvin9M[icha], "vin(9000000)" );
  ec.check( prdr->samplingFrequency() == 2.0e6, "sampling frequency" );
  if ( ec.count() == 0 ) {
    cout << line() << endl;
    cout << myname << "Draw data." << endl;
    bool usetime = true;
    TH1* phd = prdr->histdata(0, prdr->nsample(), 1000, usetime);
    TH1* phv = prdr->histvin(0, prdr->nsample(), 1000, usetime);
    ec.check( phd != nullptr, "histogram");
    if ( phd != nullptr ) {
      ostringstream sstitl;
      sstitl << dsname << " channel " << icha;
      string stitl = sstitl.str();
      phd->SetTitle(stitl.c_str());
      phd->SetMinimum(-500);
      phd->SetMaximum(4500);
      phd->SetLineWidth(2);
      phd->GetYaxis()->SetTitleOffset(1.3);
      string ylab = "ADC code, Input voltage [mV]";
      if ( phv != nullptr ) phd->GetYaxis()->SetTitle(ylab.c_str());
      TCanvas* pcan = new TCanvas;
      pcan->SetRightMargin(0.05);
      pcan->SetLeftMargin(0.13);
      pcan->SetGridy();
      phd->DrawCopy();
      if ( phv != nullptr ) {
        phv->SetLineColor(2);
        phv->SetLineWidth(2);
        phv->DrawCopy("same");
      }
      addLegend(phd, phv);
      pcan->Update();
    }
  }
  cout << line() << endl;
  cout << myname << "Check table." << endl;
  ec.check( prdr->nvin() == 20000, "nvin");
  ec.check( prdr->dvin() == 0.1, "dvin");
  ec.check( prdr->vinmin() == -200, "vinmin");
  ec.check( prdr->vinmax() == 1800, "vinmax");
  ec.checkequal( prdr->countTable()[2000][8426], 182u, "countTable()[2000][8426]");
  for ( unsigned int iv=0; iv<prdr->nvin(); ++iv ) {
    unsigned int nent = prdr->countTable()[2000][iv];
    unsigned int nfound = 0;
    if ( nent ) {
      ++nfound;
      cout << myname << "   " << iv << ": " << nent << endl;
    }
  }
  cout << line() << endl;
  if ( ec.count() ) cout << myname << "WARNING: Error count is " << ec.count() << endl;
  else cout << myname << "Done." << endl;
  cout << line() << endl;
  return ec.count();
}

int test_201703b(Index icha) {
  const string myname = "test_201703b: ";
  cout << line() << endl;
  string dsname = "201703b_D02_6b";
  cout << myname << "Testing " << dsname << endl;
  cout << line() << endl;
  ErrorCount ec(myname + "Test failed: ");
  AdcSampleFinder asf;
  cout << myname << "ADC data location: " << asf.topdir() << endl;
  cout << line() << endl;
  cout << myname << "Find sample." << endl;
  AdcSampleFinder::AdcSampleReaderPtr prdr = asf.find(dsname, icha);   // Channel 4 has 45 inconsistent ticks
  vector<SampleIndex> expNsample = {        0,        0,        0,        0,
                                     42017557, 42017808, 42014584, 42018105,
                                     42017602, 42018105, 42018561, 42016093,
                                            0,        0,        0,        0};
  vector<SampleValue> expCode20M = {    0,    0,    0,    0,
                                     1992, 2016, 2013, 2004,
                                     1988, 1958, 1951, 2008,
                                        0,    0,    0,    0};
  ec.check( prdr->dataset() == "201703b", "Dataset name" );
  ec.check( prdr->chip() == 2, "Chip" );
  ec.check( prdr->chipLabel() == "D02", "Chip label" );
  ec.check( prdr->channel() == icha, "Channel" );
  ec.check( prdr->nchannel() == 16, "# channel" );
  ec.check( prdr->nadc() == 4096, "# ADC bins" );
  ec.check( prdr->maxSample() == 0, "Max # samples" );
  ec.checkequal( prdr->nsample(), expNsample[icha], "# samples" );
  ec.checkequal( prdr->code(20000000), expCode20M[icha], "code(20M)" );
  ec.check( prdr->samplingFrequency() == 2.0e6, "sampling frequency" );
  if ( ec.count() == 0 ) {
    cout << line() << endl;
    cout << myname << "Draw data." << endl;
    bool usetime = true;
    TH1* phd = prdr->histdata(0, prdr->nsample(), 1000, usetime);
    ec.check( phd != nullptr, "histogram");
    TH1* phv = prdr->histvin(0, prdr->nsample(), 1000, usetime);
    if ( phd != nullptr ) {
      ostringstream sstitl;
      sstitl << dsname << " channel " << icha;
      string stitl = sstitl.str();
      phd->SetTitle(stitl.c_str());
      phd->SetMinimum(-500);
      phd->SetMaximum(4500);
      phd->SetLineWidth(2);
      phd->GetYaxis()->SetTitleOffset(1.3);
      string ylab = "ADC code, Input voltage [mV]";
      if ( phv != nullptr ) phd->GetYaxis()->SetTitle(ylab.c_str());
      TCanvas* pcan = new TCanvas;
      pcan->SetRightMargin(0.05);
      pcan->SetLeftMargin(0.13);
      pcan->SetGridy();
      phd->DrawCopy();
      if ( phv != nullptr ) {
        phv->SetLineColor(2);
        phv->SetLineWidth(2);
        phv->DrawCopy("same");
      }
      addLegend(phd, phv);
      pcan->Update();
    }
  }
  vector<int> expvin04M = {    0,    0,    0,    0,
                             310,  310,    0,    0,
                               0,    0,    0,    0,
                               0,    0,    0,    0};
  vector<int> expvin10M = {    0,    0,    0,    0,
                            -290, -290,    0,    0,
                               0,    0,    0,    0,
                               0,    0,    0,    0};
  vector<int> expvin20M = {    0,    0,    0,    0,
                             692,  692,    0,    0,
                               0,    0,    0,    0,
                               0,    0,    0,    0};
  ec.checkequal( int(prdr->vin( 4000000)), expvin04M[icha], "vin( 4000000)" );
  ec.checkequal( int(prdr->vin(10000000)), expvin10M[icha], "vin(10000000)" );
  ec.checkequal( int(prdr->vin(20000000)), expvin20M[icha], "vin(20000000)" );
  cout << line() << endl;
  cout << myname << "Check table." << endl;
  ec.check( prdr->nvin() == 20000, "nvin");
  ec.check( prdr->dvin() == 0.1, "dvin");
  ec.check( prdr->vinmin() == -300, "vinmin");
  ec.check( prdr->vinmax() == 1700, "vinmax");
  vector<Index> expivin = {    0,    0,    0,    0,
                            9952, 9875,    0,    0,
                               0,    0,    0,    0,
                               0,    0,    0,    0};
  vector<Index> expntab = {    0,    0,    0,    0,
                             860,  579,    0,    0,
                               0,    0,    0,    0,
                               0,    0,    0,    0};
  if ( ec.count() == 0 ) {
    Index ivin = expivin[icha];
    ostringstream sslab;
    sslab << "countTable()[2000][" << ivin << "]";
    string slab = sslab.str();
    ec.checkequal( prdr->countTable()[2000][ivin], expntab[icha], slab);
  }
  for ( unsigned int iv=0; iv<prdr->nvin(); ++iv ) {
    unsigned int nent = prdr->countTable()[2000][iv];
    unsigned int nfound = 0;
    if ( nent ) {
      ++nfound;
      cout << myname << "   " << iv << ": " << nent << endl;
    }
  }
  cout << line() << endl;
  if ( ec.count() ) cout << myname << "WARNING: Error count is " << ec.count() << endl;
  else cout << myname << "Done." << endl;
  cout << line() << endl;
  return ec.count();
}

}  // end unnamed namespace

int test_AdcSampleFinder(int icha201703a =4, int icha201703b =5) {
  const string myname = "test_AdcSampleReader: ";
  unsigned int ec = 0;
  if ( icha201703a >= 0 ) {
    ec += test_201703a(icha201703a);
    cout << "\n" << endl;
  }
  if ( icha201703b >= 0 ) {
    ec += test_201703b(icha201703b);
    cout << "\n" << endl;
  }
  if ( ec != 0 ) {
    cout << myname << "ERROR: Test failure count is " << ec << endl;
    return ec;
  }
  cout << myname << "All tests pass." << endl;
  return 0;
}

