// AdcTreeChannelCalibration.h

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
// on calCount(code) measurements with that code. Fraction in the tail
// of the distribution (e.g. pull > 5) is calTail(code).


#ifndef AdcTreeChannelCalibration_H
#define AdcTreeChannelCalibration_H

#include <vector>
#include "AdcChannelId.h"
#include "AdcTypes.h"

class AdcTreeChannelCalibration {

public:

  // Load an existing calibration from a Root TTree.
  static const AdcTreeChannelCalibration* find(std::string dataset, AdcChannelId aid);
  static const AdcTreeChannelCalibration* find(std::string dataset, ShortIndex chip, ShortIndex chan);

  // Ctors.
  AdcTreeChannelCalibration();
  AdcTreeChannelCalibration(AdcChannelId aid, AdcTime atime);
  AdcTreeChannelCalibration(AdcChannelId aid, AdcTime atime,
                        Float again, Float aoffset,
                        const FloatVector& acalMeans,
                        const FloatVector& acalRmss,
                        const ShortIndexVector& acalCounts);

  // This object is valid if it holds a valid ID.
  bool isValid() const { return id().isValid(); }

  // Return the ADC channel ID.
  AdcChannelId id() const { return AdcChannelId(chip, chan); }

  // Full calibration for each ADC code.
  Float calMean(AdcCode code) const;
  Float calRms(AdcCode code) const;
  Float calTail(AdcCode code) const;
  ShortIndex calCount(AdcCode code) const;

public:

  ShortIndex chip;
  ShortIndex chan;
  AdcTime time;
  Float gain;
  Float offset;
  FloatVector calMeans;
  FloatVector calRmss;
  FloatVector calTails;
  ShortIndexVector calCounts;

};

#endif
