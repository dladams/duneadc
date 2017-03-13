// AdcVoltagePerformance.h

// David Adams
// February 2017
//
// Class to hold performance data for an ADC channel.
//
// Performance is recorded in nv voltage bins running from vmin to vmax
// and includes the following each with one entry for each voltage bin:
//   vinCounts - # samples
//   vinEffs - Efficiency == fraction of samples that are good
//   vinRmsMeans - <Vres> = Mean(Vres) for good samples
//   vinRmsRmss - RMS(Vres) from mean of good samples
//   vinRmsRmslos - RMS(Vres) from mean of good samples with Vres < <Vres>
//   vinRmsRmshis - RMS(Vres) from mean of good samples with Vres > <Vres>
// where Vres is the input voltage resolution defined as the ultimate
// resolution for each sample bin. Vres is good if Vres < vuncmax.
// The lo and hi RMS each include half of the samples that have Vres = <Vres>
//
// The calculations of these values are performed in AdcSampleAnalyzer.cxx.

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
  AdcVoltagePerformance(Index achip, Index achan, Float avuncmax,
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
  Float eff(Index iv) const;       // Efficiency fraction good
  Float deff(Index iv) const;      // Uncertainty on efficiency
  Float resMean(Index iv) const;   // Mean good Vres
  Float resRms(Index iv) const;    // RMS good Vres
  Float resRmslo(Index iv) const;  // RMS good Vres < mean
  Float resRmshi(Index iv) const;  // RMS good Vres > mean

public:

  Index chip;
  Index chan;
  Float vuncmax;
  Index nv;
  Float vmin;
  Float vmax;
  CountVector vinCounts;
  FloatVector vinEffs;
  FloatVector vinResMeans;
  FloatVector vinResRmss;
  FloatVector vinResRmslos;
  FloatVector vinResRmshis;

};

#endif
