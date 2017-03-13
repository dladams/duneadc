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
// on calCount(code) meausurements with that code. Fraction in the tail
// of the distribution (e.g. pull > 5) is calTail(code).


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

  // Load an existing calibration from a Root TTree.
  static const AdcChannelCalibration* find(std::string dataset, AdcChannelId aid);
  static const AdcChannelCalibration* find(std::string dataset, Index chip, Index chan);

  // Ctors.
  AdcChannelCalibration();
  AdcChannelCalibration(AdcChannelId aid);
  AdcChannelCalibration(AdcChannelId aid, Float again, Float aoffset,
                        const FloatVector& acalMeans,
                        const FloatVector& acalRmss,
                        const IndexVector& acalCounts);

  // This object is valid if it holds a valid ID.
  bool isValid() const { return id().isValid(); }

  // Return the ADC channel ID.
  AdcChannelId id() const { return AdcChannelId(chip, chan); }

  // Full calibration for each ADC code.
  Float calMean(Index code) const;
  Float calRms(Index code) const;
  Float calTail(Index code) const;
  Index calCount(Index code) const;

public:

  Index chip;
  Index chan;
  Float gain;
  Float offset;
  FloatVector calMeans;
  FloatVector calRmss;
  FloatVector calTails;
  IndexVector calCounts;

};

#endif
