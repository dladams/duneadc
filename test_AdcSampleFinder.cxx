// test_AdcSampleFinder.cxx

#include "AdcSampleFinder.h"
#include "ErrorCount.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

namespace {

string line() {
  return "-----------------------------------------";
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
  ec.check( prdr->code(6000000) == 3131, "code(6000000)" );
  ec.check( prdr->vin(0) == 0, "vin(0)" );
  ec.check( prdr->vin(6000000) == 1200, "vin(6000000)" );
  ec.check( prdr->vin(9000000) == 1800, "vin(9000000)" );
  ec.check( prdr->samplingFrequency() == 2.0e6, "sampling frequency" );
  if ( ec.count() == 0 ) {
    cout << line() << endl;
    cout << myname << "Draw data." << endl;
    bool usetime = true;
    TH1* phd = prdr->histdata(0, prdr->nsample(), 1000, usetime);
    TH1* phv = prdr->histvin(0, prdr->nsample(), 1000, usetime);
    ec.check( phd != nullptr, "histogram");
    if ( phd != nullptr ) {
      phd->SetTitle(dsname.c_str());
      TCanvas* pcan = new TCanvas;
      phd->DrawCopy();
      if ( phv != nullptr ) {
        phv->SetLineColor(2);
        phv->DrawCopy("same");
      }
      pcan->Update();
    }
  }
  cout << line() << endl;
  cout << myname << "Check table." << endl;
  ec.check( prdr->nvin() == 20000, "nvin");
  ec.check( prdr->dvin() == 0.1, "dvin");
  ec.check( prdr->vinmin() == -200, "vinmin");
  ec.check( prdr->vinmax() == 1800, "vinmax");
  //ec.checkequal( prdr->countTable()[2000][8426], 182u, "countTable()[2000][8426]");
  ec.checkequal( prdr->countTable()[2000][10426], 181u, "countTable()[2000][10426]");
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
                                            0, 42017808, 42014584, 42018105,
                                     42017602, 42018105, 42018561, 42016093,
                                            0,        0,        0,        0};
  vector<SampleValue> expCode20M = {    0,    0,    0,    0,
                                        0, 2016, 2013, 2004,
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
    TH1* phv = prdr->histvin(0, prdr->nsample(), 1000, usetime);
    ec.check( phd != nullptr, "histogram");
    if ( phd != nullptr ) {
      phd->SetTitle(dsname.c_str());
      TCanvas* pcan = new TCanvas;
      phd->DrawCopy();
      if ( phv != nullptr ) {
        phv->SetLineColor(2);
        phv->DrawCopy("same");
      }
      pcan->Update();
    }
  }
  ec.check( int(prdr->vin( 4000000)) ==  410, "vin( 4000000)" );
  ec.check( int(prdr->vin(10000000)) == -190, "vin(10000000)" );
  ec.check( int(prdr->vin(20000000)) ==  792, "vin(20000000)" );
  cout << line() << endl;
  cout << myname << "Check table." << endl;
  ec.check( prdr->nvin() == 20000, "nvin");
  ec.check( prdr->dvin() == 0.1, "dvin");
  ec.check( prdr->vinmin() == -200, "vinmin");
  ec.check( prdr->vinmax() == 1800, "vinmax");
  if ( ec.count() == 0 ) {
    ec.checkequal( prdr->countTable()[2000][9875], 579u, "countTable()[2000][9875]");
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

