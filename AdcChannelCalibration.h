// AdcChannelCalibration.h

// David Adams
// June 2017
//
// Interface for a tool that provides calibration data for an ADC channel.

#ifndef AdcChannelCalibration_H
#define AdcChannelCalibration_H

#include "AdcTypes.h"

class AdcChannelCalibration {

public:

  // Dtor.
  virtual ~AdcChannelCalibration() =default;

  // Return if this is a valid calibration.
  virtual bool isValid() const { return true; }

  // Return a name for this calibration.
  virtual std::string name() const { return "UnnamedCalib"; }

  // Return the sample name for this calibration.
  virtual std::string sample() const { return "UnspecifiedSample"; }

  // Return the chip (or other context index) for this channel.
  virtual Index chip() const { return 0; }

  // Return the channel number (within the context) for this channel.
  virtual Index channel() const =0;

  // Return the time. E.g. the time at which theis calibration becomes valid.
  // Typically this is unix time (seconds since 1970).
  virtual Index time() const { return 0; }

  // Return the number of ADC codes (bins).
  virtual Index size() const =0;

  // Return the calibrated voltage (typically mV) for an ADC code.
  virtual double calMean(AdcCode code) const =0;

  // Return the RMS of the calibration (same units as calMean) for an ADC code.
  virtual double calRms(AdcCode code) const { return 0.0; }

  // Return the tail fraction for an ADC code.
  // This is the fraction of samples that are populated by values far from the mean.
  // E.g. |Vcal - Vtrue| < 5 mV.
  virtual double calTail(AdcCode code) const { return 0.0; }

  // Return an approximate linear calibration: V = linearOffset + linearGain*code
  virtual double linearOffset() const { return 0.0; }
  virtual double linearGain() const { return 0.0; }

};

#endif
