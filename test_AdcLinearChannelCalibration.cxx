// test_AdcLinearChannelCalibration.cxx

#include "AdcLinearChannelCalibration.h"
#include "ErrorCount.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

int test_AdcLinearChannelCalibration() {
  const string myname = "test_AdcLinearChannelCalibration: ";
  ErrorCount ec(myname + "ERROR: Test failed: ");
  cout << myname << "Creating calibration." << endl;
  double gain = 2.0;
  double offset = 10.0;
  Index nbin = 4096;
  Index chan = 111;
  Index chip = 22;
  Index time = 123456;
  AdcLinearChannelCalibration cal(gain, offset, nbin, chan, chip, time);
  cout << "Testing configuration." << endl;
  ec.check(cal.linearGain() == gain, "Checking gain");
  ec.check(cal.linearOffset() == offset, "Checking offset");
  ec.check(cal.size() == nbin, "Checking size");
  ec.check(cal.channel() == chan, "Checking channel");
  ec.check(cal.chip() == chip, "Checking chip");
  ec.check(cal.time() == time, "Checking time");
  cout << "Testing calibration." << endl;
  ec.check(cal.calMean(21)    ==   52, "calMean(21)");
  ec.check(cal.calMean(121)   ==  252, "calMean(121)");
  ec.check(cal.calMean(1021)  == 2052, "calMean(1021)");
  ec.check(cal.calMean(10021) ==    0, "calMean(10021)");
  cout << "Testing RMS." << endl;
  ec.check(cal.calRms(21)    == 0, "calRms(21)");
  ec.check(cal.calRms(121)   == 0, "calRms(121)");
  cout << myname << "Done." << endl;
  return ec.count();
}

