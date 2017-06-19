// test_AdcBinRecord

#include "AdcBinRecord.h"
#include "ErrorCount.h"
#include <iostream>

using std::cout;
using std::endl;

int test_AdcBinRecord() {
  const string myname = "test_AdcBinRecord: ";
  ErrorCount ec(myname + "ERROR: Test failed: ");
  cout << myname << "Creating recorder." << endl;
  AdcCode code = 1234;
  AdcBinRecord abr(code);
  ec.check(abr.code() == code, "abr.code() == code");
  ec.check(abr.samples().size() == 0, "abr.samples().size() == 0");
  ec.check(abr.peaks().size() == 0, "abr.peaks().size() == 0");
  cout << myname << "Adding samples." << endl;
  string msg = "Add good sample.";
  ec.check(abr.addSample(11) == 0, msg);
  ec.check(abr.addSample(12) == 0, msg);
  ec.check(abr.addSample(13) == 0, msg);
  ec.check(abr.addSample(14) == 0, msg);
  ec.check(abr.addSample(15) == 0, msg);
  cout << myname << "Adding out-of-order samples." << endl;
  ec.check(abr.addSample(5) != 0, "Add bad sample");
  cout << myname << "Checking samples." << endl;
  ec.check(abr.samples().size() == 5, "abr.samples().size() == 5");
  ec.check(abr.samples()[0] == 11, "abr.samples()[0] == 11");
  ec.check(abr.samples()[4] == 15, "abr.samples()[4] == 15");
  cout << myname << "Adding more samples." << endl;
  ec.check(abr.addSample(21) == 0, msg);
  ec.check(abr.addSample(22) == 0, msg);
  ec.check(abr.addSample(27) == 0, msg);
  ec.check(abr.addSample(28) == 0, msg);
  cout << myname << "Checking samples." << endl;
  ec.check(abr.samples().size() == 9, "abr.samples().size() == 9");
  ec.check(abr.samples()[8] == 28, "abr.samples()[8] == 28");
  cout << myname << "Finding peaks." << endl;
  ec.check(abr.findPeaks(5) == 2, "abr.findPeaks(5) == 2");
  cout << myname << "Checking peaks." << endl;
  cout << myname << "  minGap: " << abr.minGap() << endl;
  cout << myname << " # peaks: " << abr.npeak() << endl;
  ec.check(abr.minGap() == 5, "abr.minGap() == 5");
  ec.check(abr.npeak() == 2, "abr.npeak() == 2");
  ec.check(abr.peaks().size() == 2, "abr.peaks().size() == 2");
  ec.check(abr.peak(0).size ==  5,        "abr.peak(0).size");
  ec.check(abr.peak(0).low  == 11,        "abr.peak(0).low");
  ec.check(abr.peak(0).high == 15,        "abr.peak(0).high");
  ec.check(abr.peak(0).meanOffset == 2.0, "abr.peak(0).meanOffset");
  ec.check(abr.peak(0).truncMean == 13,   "abr.peak(0).truncMean");
  ec.check(abr.peak(0).mean() == 13.0,    "abr.peak(0).mean()");
  ec.check(abr.peak(0).width() == 4,      "abr.peak(0).width()");
  ec.check(abr.peak(1).size ==  4,        "abr.peak(1).size");
  ec.check(abr.peak(1).low  == 21,        "abr.peak(1).low");
  ec.check(abr.peak(1).high == 28,        "abr.peak(1).high");
  ec.check(abr.peak(1).meanOffset == 3.5, "abr.peak(1).meanOffset");
  ec.check(abr.peak(1).truncMean == 24,   "abr.peak(1).truncMean");
cout << myname << "Peak 1 size: " << abr.peak(1).size << endl;
  cout << myname << "Done." << endl;
  return ec.count();
}

