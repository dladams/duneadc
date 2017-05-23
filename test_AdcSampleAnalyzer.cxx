// test_AdcSampleAnalyzer.cxx

#include "AdcSampleAnalyzer.h"
#include "AdcSampleFinder.h"

using std::string;

int test_AdcSampleAnalyzer(string dscalib ="linear", double nomGain =0.342) {
  const string myname = "test_AdcSampleAnalyzer: ";
  const string line = "----------------------------------------------------------------";
  cout  << myname << line << endl;
  AdcSampleFinder asf;
  cout << myname << "Finding sample." << endl;
  auto prdr = asf.find("201703b_D02_6b", 4);
  cout  << myname << line << endl;
  cout << myname << "Analyzing sample with calibration " << dscalib << "." << endl;
  AdcSampleAnalyzer asa(*prdr, dscalib, nomGain);
  asa.phn->Draw("colz");
  cout  << myname << line << endl;
  cout << myname << "All tests pass." << endl;
  cout  << myname << line << endl;
  return 0;
}
