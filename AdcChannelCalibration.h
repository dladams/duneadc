// AdcChannelCalibration.h

// David Adams
// January 2017
//
// Class to hold the calibration data for an ADC channel.
//
// Linear calibration is
//   V = gain*code + offset
// where code is the ADC code (0-4095) and V and offset are in mV.
//
// Full calibrations is calMean(code) with RMS calRms(code). It is based
// on calCount(code) meausurements with that code.


#ifndef AdcChannelCalibration_H
#define AdcChannelCalibration_H

#include <vector>
#include "AdcChannelId.h"

class AdcChannelCalibration {

public:

  using Index = unsigned short;
  using Float = float;
  using IndexVector = std::vector<Index>;
  using FloatVector = std::vector<Float>;

  // Ctors.
  AdcChannelCalibration() = default;
  AdcChannelCalibration(AdcChannelId aid, Float again, Float aoffset,
                        const FloatVector& acalMeans,
                        const FloatVector& acalRmss,
                        const IndexVector& acalCounts);

  // This object is valid if it holds a valid ID.
  bool isValid() const { return id.isValid(); }

  // Full calibration for each ADC code.
  Float calMean(Index code) const;
  Float calRms(Index code) const;
  Index calCount(Index code) const;

public:

  AdcChannelId id;
  Float gain;
  Float offset;
  FloatVector calMeans;
  FloatVector calRmss;
  IndexVector calCounts;

};

#endif
