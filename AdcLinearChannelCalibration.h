// AdcLinearChannelCalibration.h

// David Adams
// June 2017
//
// Class to hold a linear ADC channel calibration:
//   V = gain*iadc + offset

#ifndef AdcLinearChannelCalibration_H
#define AdcLinearChannelCalibration_H

#include "AdcChannelCalibration.h"
#include <vector>
#include "AdcChannelId.h"
#include "AdcTypes.h"

class AdcLinearChannelCalibration : public AdcChannelCalibration {

public:

  // Ctor from gain and offset.
  AdcLinearChannelCalibration(double gain, double offset, Index nbin =4096,
                              Index chan =0, Index chip =0, Index time =0);

  // Interface methods.
  bool isValid()               const override { return size() > 0; }
  Index chip()                 const override { return m_chip; }
  Index channel()              const override { return m_chan; }
  Index time()                 const override { return m_time; }
  Index size()                 const override { return m_size; }
  double calMean(AdcCode code) const override;
  double calRms(AdcCode code)  const override { return 0.0; }
  double calTail(AdcCode code) const override { return 0.0; }
  double linearOffset()        const override { return m_offset; }
  double linearGain()          const override { return m_gain; }

private:

  Index m_size;
  Index m_chip;
  Index m_chan;
  Index m_time;
  double m_gain;
  double m_offset;

};

#endif
