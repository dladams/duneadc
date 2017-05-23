// test_AdcSampleAnalyzer.cxx

#include "AdcSampleAnalyzer.h"
#include "AdcSampleFinder.h"
#include "TCanvas.h"

using std::string;

TCanvas* hover(TH1* ph1, TH1* ph2) {
  TCanvas* pcan = new TCanvas;
  ph2->SetLineColor(2);
  ph2->SetLineStyle(2);
  ph1->Draw();
  ph2->Draw("same");
  return pcan;
}
  

int test_AdcSampleAnalyzer(string dscalib ="linear", double nomGain =0.342) {
  const string myname = "test_AdcSampleAnalyzer: ";
  const string line = "----------------------------------------------------------------";
  cout  << myname << line << endl;
  AdcSampleFinder asf;
  cout << myname << "Finding sample." << endl;
  string sampleName = "201703b_D02_6b";
  auto prdr = asf.find(sampleName, 4);
  cout  << myname << line << endl;
  cout << myname << "Analyzing sample from reader." << endl;
  AdcSampleAnalyzer asa(*prdr, dscalib, nomGain);
  TCanvas* pcan = new TCanvas;
  asa.phn->Draw("colz");
  cout << myname << line << endl;
  cout << myname << "Analyzing sample from calibration." << endl;
  sampleName += "_calib";
  AdcSampleAnalyzer asa2(asa.dataset(), sampleName, asa.calib());
  cout << myname << line << endl;
  cout << myname << "Comparing results from reader and calibration." << endl;
  hover(asa.phm, asa2.phm);
  hover(asa.phs, asa2.phs);
  hover(asa.phsg, asa2.phsg);
  hover(asa.phsb, asa2.phsb);
  hover(asa.phr, asa2.phr);
  hover(asa.pht, asa2.pht)->SetLogy();
  hover(asa.phdr, asa2.phdr);
  hover(asa.phds, asa2.phds);
  hover(asa.phdsb, asa2.phdsb);
  cout  << myname << line << endl;
  cout << myname << "All tests pass." << endl;
  cout  << myname << line << endl;
  return 0;
}
