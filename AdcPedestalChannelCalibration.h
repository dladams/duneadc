// AdcPedestalChannelCalibration.h

// David Adams
// June 2017
//
// Class to subtract a pedestal from an ADC channel calibration.
//
// The pedestal may be supplied directly or may be calculated over the range
//   iadc1 <= iadc < iadc2
// so the average over that range is vped.
// Bins with RMS > rmsmax are ignored.

#ifndef AdcPedestalChannelCalibration_H
#define AdcPedestalChannelCalibration_H

#include "AdcChannelCalibration.h"
#include <vector>
#include "AdcChannelId.h"
#include "AdcTypes.h"

class AdcPedestalChannelCalibration : public AdcChannelCalibration {

public:

  // Ctor from specified pedestal.
  AdcPedestalChannelCalibration(const AdcChannelCalibration& calraw, double ped);

  // Ctor from specified voltage.
  AdcPedestalChannelCalibration(const AdcChannelCalibration& calraw,
                                Index iadc1, Index iadc2,
                                double vped =0.0,
                                double rmsmax =1.0);

  // Getters.
  const AdcChannelCalibration& rawCalibration() const { return m_calraw; }
  double pedestal() const { return m_ped; }

  // This object is valid if it holds a valid ID.
  bool isValid()               const override { return rawCalibration().isValid(); }
  Index chip()                 const override { return rawCalibration().chip(); }
  Index channel()              const override { return rawCalibration().channel(); }
  Index time()                 const override { return rawCalibration().time(); }
  Index size()                 const override { return rawCalibration().size(); }
  double calMean(AdcCode code) const override;
  double calRms(AdcCode code)  const override { return rawCalibration().calRms(code); }
  double calTail(AdcCode code) const override { return rawCalibration().calTail(code); }
  double linearOffset()        const override { return rawCalibration().linearOffset() - pedestal(); }
  double linearGain()          const override { return rawCalibration().linearGain(); }

private:

  const AdcChannelCalibration& m_calraw;
  double m_ped;

};

#endif
