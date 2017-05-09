// test_AdcSampleFinder.cxx

#include "AdcSampleFinder.h"
#include "ErrorCount.h"
#include "TH1.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

namespace {

string line() {
  return "-----------------------------------------";
}

int test_201703a() {
  const string myname = "test_201703a: ";
  cout << line() << endl;
  string dsname = "201703a_D22";
  cout << myname << "Testing " << dsname << endl;
  cout << line() << endl;
  ErrorCount ec(myname + "Test failed: ");
  AdcSampleFinder asf;
  AdcSampleFinder::AdcSampleReaderPtr prdr = asf.find(dsname, 4);
  ec.check( prdr->dataset() == "201703a", "Dataset name" );
  ec.check( prdr->chip() == 22, "Chip index" );
  ec.check( prdr->chipLabel() == "D22", "Chip label" );
  ec.check( prdr->channel() == 4, "Channel" );
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
    TH1* ph = prdr->histdata(0, prdr->nsample(), 1000);
    ec.check( ph != nullptr, "histogram");
    if ( ph != nullptr ) {
      ph->SetTitle(dsname.c_str());
      ph->Draw();
    }
  }
  cout << line() << endl;
  cout << myname << "Check table." << endl;
  ec.check( prdr->nvin() == 20000, "nvin");
  ec.check( prdr->dvin() == 0.1, "dvin");
  ec.check( prdr->vinmin() == -200, "vinmin");
  ec.check( prdr->vinmax() == 1800, "vinmax");
  ec.check( prdr->countTable()[2000][8426] == 182, "countTable()[2000][8426]");
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

int test_201703b() {
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
  AdcSampleFinder::AdcSampleReaderPtr prdr = asf.find(dsname, 5);   // Channel 4 has 45 inconsistent ticks
  ec.check( prdr->dataset() == "201703b", "Dataset name" );
  ec.check( prdr->chip() == 2, "Chip" );
  ec.check( prdr->chipLabel() == "D02", "Chip label" );
  ec.check( prdr->channel() == 5, "Channel" );
  ec.check( prdr->nchannel() == 16, "# channel" );
  ec.check( prdr->nadc() == 4096, "# ADC bins" );
  ec.check( prdr->maxSample() == 0, "Max # samples" );
  ec.check( prdr->nsample() == 42017808, "# samples" );
  ec.check( prdr->code(20000000) == 2016, "code(6000000)" );
  ec.check( prdr->samplingFrequency() == 2.0e6, "sampling frequency" );
  if ( ec.count() == 0 ) {
    cout << line() << endl;
    cout << myname << "Draw data." << endl;
    TH1* ph = prdr->histdata(0, prdr->nsample(), 1000);
    ec.check( ph != nullptr, "histogram");
    if ( ph != nullptr ) {
      ph->SetTitle(dsname.c_str());
      ph->Draw();
    }
  }
  ec.check( prdr->vin(0) == 0, "vin(0)" );
  ec.check( prdr->vin(6000000) == 1200, "vin(6000000)" );
  ec.check( prdr->vin(9000000) == 1800, "vin(9000000)" );
  cout << line() << endl;
  cout << myname << "Check table." << endl;
  ec.check( prdr->nvin() == 20000, "nvin");
  ec.check( prdr->dvin() == 0.1, "dvin");
  ec.check( prdr->vinmin() == -200, "vinmin");
  ec.check( prdr->vinmax() == 1800, "vinmax");
  //ec.check( prdr->countTable()[2000][8426] == 182, "countTable()[2000][8426]");
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

int test_AdcSampleFinder() {
  const string myname = "test_AdcSampleReader: ";
  unsigned int ec = 0;
  //ec += test_201703a();
  cout << "\n" << endl;
  ec += test_201703b();
  cout << "\n" << endl;
  if ( ec != 0 ) {
    cout << myname << "ERROR: Test failure count is " << ec << endl;
    return ec;
  }
  cout << myname << "All tests pass." << endl;
  return 0;
}

