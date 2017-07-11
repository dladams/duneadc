// AdcChipMetric
//
// Evaluates channel and chip metrics.
// 
// Virtual so subclass my implement a different metric in evalaute.

#ifndef AdcChipMetric_H
#define AdcChipMetric_H

#include <string>
#include <vector>
#include <map>

class AdcChipMetric {

public:

  using Name = std::string;
  using Index = unsigned int;
  using DoubleVector = std::vector<double>;
  using MetricMap = std::map<Name, double>;

  // Ctor from dataset name, chip and channel range.
  //   dataset = dataset name (performance file is perf_dataset.root)
  //   chip = chip number
  //   a_firstChannel = first channel number
  //   a_nChannel = number of channels
  AdcChipMetric(Name a_dataset, Index a_chip, Index a_firstChannel =0, Index a_nChannel =16);

  // Ctor from dataset name, sample name and channel range.
  //   dataset = dataset name (performance file is perf_dataset.root)
  //   chip = chip number
  //   a_firstChannel = first channel number
  //   a_nChannel = number of channels
  AdcChipMetric(Name a_dataset, Name a_sampleName, Index a_firstChannel =0, Index a_nChannel =16);

  // Dtor;
  virtual ~AdcChipMetric() =default;

  // Evaluate the metric.
  // Implementation here uses the average efficiency over 200 < Vin < 1200 mV
  // for each channel and the product of these for the chip.
  virtual int evaluate();

  // Return the chip description.
  Name sampleName() const { return m_sampleName; }
  Name dataset() const { return m_dataset; }
  Index chip() const { return m_chip; }
  Index firstChannel() const { return m_firstChannel; }
  Index nChannel() const { return m_nChannel; }

  // Return the mean efficiency for each channel.
  double channelEfficiency(Index chan) const;

  // Return the value of a metric for the chip.
  //   name - metric name
  double chipMetric(Name name) const;

  // Return the map of chip metrics.
  const MetricMap& chipMetrics() const { return m_chipMetric; }

  // Display results.
  void print() const;

protected:

  // Setters.
  int setChannelEfficiency(Index chan, double val);
  int setChipMetric(Name, double val);

private:

  Name m_dataset;
  Name m_sampleName;
  Index m_chip;
  Index m_firstChannel;
  Index m_nChannel;
  DoubleVector m_chanEff;
  MetricMap m_chipMetric;

};

#endif
