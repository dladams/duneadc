// AdcExtremaFinder.h
//
// Interface for finding extrema in an ADC waveform.

#ifndef AdcExtremaFinder_H
#define AdcExtremaFinder_H

#include "AdcTypes.h"
#include "AdcSampleReader.h"
#include "AdcExtremum.h"

class AdcExtremaFinder {

public:

  using AdcCode = unsigned short;

public: // Subclass describing a range of samples.

  // Dtor.
  virtual ~AdcExtremaFinder() =default;

  // Find all the extrema in a waveform.
  // The extremum vector is first cleared.
  virtual int find(const AdcSampleReader& reader, AdcExtrema& ext) const =0;

};

#endif
