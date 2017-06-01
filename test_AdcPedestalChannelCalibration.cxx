// test_AdcPedestalChannelCalibration.cxx

#include "AdcPedestalChannelCalibration.h"
#include "AdcLinearChannelCalibration.h"
#include "ErrorCount.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

int test_AdcPedestalChannelCalibration() {
  const string myname = "test_AdcPedestalLinearChannelCalibration: ";
  ErrorCount ec(myname + "ERROR: Test failed: ");
  cout << myname << "Creating calibration." << endl;
  double gain = 2.0;
  double offset = 15.0;
  double pedestal = 5.0;
  double linoff = offset - pedestal;
  Index nbin = 4096;
  Index chan = 111;
  Index chip = 22;
  Index time = 123456;
  AdcLinearChannelCalibration callin(gain, offset, nbin, chan, chip, time);
  AdcPedestalChannelCalibration cal(callin, pedestal);
  cout << "Testing configuration." << endl;
  ec.check(cal.linearGain() == gain, "Checking gain");
  ec.check(cal.linearOffset() == linoff, "Checking offset");
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

