// test_AdcChipAnalyzer.cxx

// David Adams
// May 2017

// Test for AdcChipAnalyzer.

#include "AdcChipAnalyzer.h"

int test_AdcChipAnalyzer(Index ncha) {
  AdcChipAnalyzer chipper("201703b_D02_6b", 0, ncha, "", false, -200, 1600, 90, 1.0, false, false);
  bool save = false;
  chipper.draw("rawv", save);
  chipper.draw("resp", save);
  chipper.draw("zres", save);
  chipper.draw("diff", save);
  chipper.draw("frms", save);
  chipper.draw("fsdv", save);
  chipper.draw("fsdt", save);
  chipper.draw("perf", save);
  chipper.draw("sumfdr", save);
  chipper.draw("gain", save);
  chipper.draw("offset", save);
  return 0;
}

