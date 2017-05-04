// AdcBorderExtremaFinder.cxx

#include "AdcBorderExtremaFinder.h"

//**********************************************************************

AdcBorderExtremaFinder::
AdcBorderExtremaFinder(SampleIndex fenceWidth,
                       SampleValue minThresh, SampleValue maxThresh,
                       SampleValue minLimit, SampleValue maxLimit)
: m_borderFinder(fenceWidth, minThresh, maxThresh, minLimit, maxLimit) { }

//**********************************************************************

int AdcBorderExtremaFinder::
find(const AdcSampleReader& reader, Extrema& ext) const {
  const string myname = "AdcBorderExtremaFinder::find: ";
  ext.clear();
  AdcBorderFinder::SampleRangeVector borders;
  borderFinder().find(reader, borders);
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
