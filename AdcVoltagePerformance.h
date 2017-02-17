// AdcVoltagePerformance.h

// David Adams
// February 2017
//
// Class to hold performance data for an ADC channel.
//
// Performance is recorded in nv voltage bins running from vmin to vmax
// and includes the following
//   vinCounts - # samples in each bin
//   vinEffs - Efficiency in each bin for RMS < vrmsmax

#ifndef AdcVoltagePerformance_H
#define AdcVoltagePerformance_H

#include <vector>
#include "AdcChannelId.h"

class AdcVoltagePerformance {

public:

  using Index = unsigned short;
  using Count = unsigned int;
  using Float = float;
  using CountVector = std::vector<Count>;
  using FloatVector = std::vector<Float>;

  // Ctors.
  AdcVoltagePerformance();
  AdcVoltagePerformance(AdcChannelId aid);
  AdcVoltagePerformance(Index achip, Index achan, Float avrmsmax,
                        Index anv, Float avmin, Float avmax);

  // This object is valid if it holds a valid ID.
  bool isValid() const { return id().isValid(); }

  // Return the ADC channel ID.
  AdcChannelId id() const { return AdcChannelId(chip, chan); }

  // Bin limits.
  Float vlo(Index iv) const;
  Float vhi(Index iv) const;

  // Performance for each voltage bin.
  Count count(Index iv) const;
  Float eff(Index iv) const;
  Float deff(Index iv) const;     // Uncertainty on efficiency

public:

  Index chip;
  Index chan;
  Float vrmsmax;
  Index nv;
  Float vmin;
  Float vmax;
  CountVector vinCounts;
  FloatVector vinEffs;

};

#endif
