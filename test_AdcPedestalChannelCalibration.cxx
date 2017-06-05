// test_AdcPedestalChannelCalibration.cxx

#include "AdcPedestalChannelCalibration.h"
#include "AdcLinearChannelCalibration.h"
#include "ErrorCount.h"
#include <iostream>

using std::string;
using std::cout;
using std::endl;

namespace {

double gain() { return 2.0; }
double offset() { return 15.0; }
double pedestal() { return 5.0; }
double linoff() { return offset() - pedestal(); }
Index nbin() { return 4096; }
Index chan() { return 111; }
Index chip() { return 22; }
Index time() { return 123456; }

}

void checkCal(ErrorCount& ec, const AdcChannelCalibration& cal) {
  const string myname = "checkCal: ";
  cout << myname << "..Testing configuration." << endl;
  ec.check(cal.linearGain() == gain(), "Checking gain");
  ec.check(cal.linearOffset() == linoff(), "Checking offset");
  ec.check(cal.size() == nbin(), "Checking size");
  ec.check(cal.channel() == chan(), "Checking channel");
  ec.check(cal.chip() == chip(), "Checking chip");
  ec.check(cal.time() == time(), "Checking time");
  cout << myname << "..Testing calibration." << endl;
  ec.checkequal(cal.calMean(21),  52, "calMean(21)");
  ec.checkequal(  cal.calMean(121),  252, "calMean(121)");
  ec.checkequal( cal.calMean(1021), 2052, "calMean(1021)");
  ec.checkequal(cal.calMean(10021),    0, "calMean(10021)");
  cout << myname << "..Testing RMS." << endl;
  ec.check(cal.calRms(21)    == 0, "calRms(21)");
  ec.check(cal.calRms(121)   == 0, "calRms(121)");
}

int test_AdcPedestalChannelCalibration() {
  const string myname = "test_AdcPedestalLinearChannelCalibration: ";
  ErrorCount ec(myname + "ERROR: Test failed: ");
  cout << myname << "Creating calibration." << endl;
  AdcLinearChannelCalibration callin(gain(), offset(), nbin(), chan(), chip(), time());
  AdcPedestalChannelCalibration cal1(callin, pedestal());
  cout << myname << "Construction from pedestal." << endl;
  cout << myname << "  pedestal = " << cal1.pedestal() << endl;
  checkCal(ec, cal1);
  cout << myname << "Construction from calib." << endl;
  AdcPedestalChannelCalibration cal2(callin, 290, 311, cal1, 1.0);
  cout << myname << "  pedestal = " << cal2.pedestal() << endl;
  checkCal(ec, cal2);
  cout << myname << "Done." << endl;
  return ec.count();
}

