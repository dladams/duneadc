// AdcBorderExtremaFinder.cxx

#include "AdcBorderExtremaFinder.h"

//**********************************************************************

AdcBorderExtremaFinder::
AdcBorderExtremaFinder(SampleIndex fenceWidth,
                       SampleValue minThresh, SampleValue maxThresh,
                       SampleValue minLimit, SampleValue maxLimit,
                       bool useMitigated)
: m_borderFinder(fenceWidth, minThresh, maxThresh, minLimit, maxLimit, useMitigated) { }

//**********************************************************************

int AdcBorderExtremaFinder::
find(const AdcSampleReader& reader, AdcExtrema& ext) const {
  const string myname = "AdcBorderExtremaFinder::find: ";
  ext.clear();
  AdcBorderFinder::SampleRangeVector borders;
  int rstat = borderFinder().find(reader, borders);
  if ( rstat ) return rstat;
  for ( const AdcBorderFinder::SampleRange& border : borders ) {
    SampleIndex tick = (border.begin() + border.end())/2;
    if ( border.state() == AdcBorderFinder::UNDER ) {
      ext.emplace_back(tick, true);
    } else if ( border.state() == AdcBorderFinder::OVER ) {
      ext.emplace_back(tick, false);
    } else {
      cout << myname << "Skipping border in unexpected state." << endl;
    }
  }
  return 0;
}

//**********************************************************************
