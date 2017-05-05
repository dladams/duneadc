// AdcBorderExtremaFinder.h
//
// Tool to find ADC sample extrema using borders.

#ifndef AdcBorderExtremaFinder_H
#define AdcBorderExtremaFinder_H

#include "AdcExtremaFinder.h"
#include "AdcSampleReader.h"
#include "AdcExtremum.h"
#include "AdcBorderFinder.h"

class TTree;
class TCanvas;

class AdcBorderExtremaFinder : public AdcExtremaFinder {

public:

  using SampleIndex = AdcBorderFinder::SampleIndex;
  using SampleValue = AdcBorderFinder::SampleValue;

  // Ctor.
  // Ctor from configuration parameters.
  // See AdcBorderFinder.h
  AdcBorderExtremaFinder(SampleIndex fenceWidth,
                         SampleValue minThresh, SampleValue maxThresh,
                         SampleValue minLimit, SampleValue maxLimit);

  // Dtor.
   ~AdcBorderExtremaFinder() =default;

  // Find all the extrema in a waveform.
  // The extremum vector is first cleared.
  int find(const AdcSampleReader& reader, AdcExtrema& ext) const override;

  // Return the border finder.
  const AdcBorderFinder borderFinder() const { return m_borderFinder; }

private:

  AdcBorderFinder m_borderFinder;

};

#endif
