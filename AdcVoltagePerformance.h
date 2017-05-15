// AdcVoltagePerformance.h

// David Adams
// February 2017
//
// Class to hold performance data for an ADC channel.
//
// Data include
//   chip -  Chip index
//   chan -  Channel number
//   time - Time (unix sec) data was taken
//   vuncmax - Threshold for declaring channels bad (mV).
//   nv - # voltage bins
//   vmin - Min voltage
//   vmax - Max voltge
// with the following data for each bin:
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
#include "AdcTypes.h"
#include "AdcChannelId.h"

class AdcVoltagePerformance {

public:

  using Count = unsigned int;
  using Float = float;
  using CountVector = std::vector<Count>;
  using FloatVector = std::vector<Float>;

  // Ctors.
  AdcVoltagePerformance();
  AdcVoltagePerformance(AdcChannelId aid);
  AdcVoltagePerformance(Index a_chip, Index a_chan, AdcTime a_time,
                        Float a_vuncmax,
                        Index a_nv, Float a_vmin, Float a_vmax);

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

  Index chip;                  // Chip index
  Index chan;                  // Channel number
  AdcTime time;                // Time (unix sec) data was taken
  Float vuncmax;               // Threshold for declaring channels bad (mV).
  Index nv;                    // # voltage bins
  Float vmin;                  // Min volatage
  Float vmax;                  // Max voltage
  CountVector vinCounts;       // Chip index
  FloatVector vinEffs;                  // Chip index
  FloatVector vinResMeans;                  // Chip index
  FloatVector vinResRmss;                  // Chip index
  FloatVector vinResRmslos;                  // Chip index
  FloatVector vinResRmshis;                  // Chip index

};

#endif
