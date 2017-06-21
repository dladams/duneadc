// test_AdcFembTreeSampleReader.cxx

#include "AdcFembTreeSampleReader.h"
#include "AdcSampleFinder.h"
#include "ErrorCount.h"
#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

namespace {

string line() {
  return "-----------------------------------------";
}

}

int test_AdcFembTreeSampleReader() {

  const string myname = "test_AdcFembTreeSampleReader: ";
  ErrorCount ec(myname + "Test failed: ");

  cout << line() << endl;
  cout << myname << "Opening file." << endl;
  string fname = "adcTestData_20170613T172751_chip26_adcClock1_adcOffset-1_sampleRate2000000_functype3_freq734.000_offset0.700_amplitude1.000_calib.root";
  string fullname = AdcSampleFinder::defaultTopdir() + "/justin2/" + fname;
  Index chan = 3;
  AdcFembTreeSampleReader rdr(fullname, chan);
  ec.check(rdr.inputFile(), "inputFile()");
  ec.check(rdr.inputTree(), "inputTree()");
  cout << myname << "Data count: " << rdr.nsample() << endl;
  ec.checkequal(rdr.channel(), chan, "inputTree()");
  ec.checkequal(rdr.nsample(), 50400u, "nsample");

  cout << line() << endl;
  cout << myname << "Check values." << endl;
  ec.checkequal(rdr.code(500), 1108, "code[500]");
  ec.checkequal(int(rdr.vin(500)), 356, "vin[500]");

  cout << line() << endl;
  cout << myname << "Draw waveform." << endl;
  Index t0 = 0;
  Index nt = 5000;
  Index rebin = 1;
  Index usetime = false;
  TH1* phd = rdr.histdata(t0, nt, rebin, usetime);
  TH1* phv = rdr.histvin(t0, nt, rebin, usetime);
  TCanvas* pcan = new TCanvas;
  pcan->SetLeftMargin(0.12);
  pcan->SetRightMargin(0.03);
  pcan->SetGridx();
  pcan->SetGridy();
  phd->SetMinimum(-500);
  phd->GetYaxis()->SetTitleOffset(1.3);
  phv->SetLineColor(2);
  phd->Draw();
  phv->Draw("same");

  cout << line() << endl;
  return ec.count();
}
