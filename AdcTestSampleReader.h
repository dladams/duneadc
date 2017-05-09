// AdcTestSampleReader.h
//
// David Adams
// April 2017
//
// Class to read ADC sample data into memory.
// Data formats are those from BNL physics test data.
// The sample is specified by a name and channel number.
// The name specifies the date, conditions and chip.
//
// Early data is table (Vin vs. ADC bin) format and later is
// a waveform, i.e. series of ADC samples. In all cases, Vin increases
// linearly with time (tick). The Vin calibration is hardwired into the
// the code for each known sample. This calibration comprises an
// offset (Vin for the first sample) and slope (dVin/dt).
//
// The data is summarized in a table giving the number of samples for each
// input voltage and output ADC code.
//
// Where the input data is in that format, the data here also includes the
// raw waveform, i.e. the ADC code for each tick, along with a paramterization
// giving the input voltage for that tick.

#ifndef AdcTestSampleReader_H
#define AdcTestSampleReader_H

#include "AdcSampleReader.h"

class AdcTestSampleReader : public AdcSampleReader {

public:

  // Construct a reader that can handle any channel.
  // Call setChannel to read data for  channel.
  //  asample - sample name, e.g. "201703a_D20"
  explicit AdcTestSampleReader(Name asample);

  // Construct a reader for a particular channel.
  // Read in the data.
  //  asample - sample name, e.g. "201703a_D20"
  //  icha - channel number
  //  maxsam - maximum # samples to read from a waveform (0 for all)
  AdcTestSampleReader(Name asample, Index icha, Index maxsam =0);

  // Dtor;
  ~AdcTestSampleReader() = default;

  // Set the channel and read the data for that channel.
  int setChannel(Index icha) override;

  // Sample name.
  Name sample() const override { return m_sample; }

  // Dataset name.
  // Derived from sample name and typically excludes the chip number.
  Name dataset() const override { return m_dataset; }

  // Chip number.
  Index chip() const override { return m_chip; }

  // Chip label.
  Name chipLabel() const override { return m_chipLabel; }

  // Channel number on the chip.
  Index channel() const override { return m_chan; }

  // # channels on the chip.
  Index nchannel() const override { return 16; }

  // Number of ADC codes (4096 for the typical 12-bits)
  Code nadc() const override { return m_nadc; }

public:  // Waveform info.

  // Maximum # samples to read. 0 for all.
  Index maxSample() const override { return m_maxSample; }

  // The total number of samples.
  // Same as data.size() for a waveform.
  SampleIndex nsample() const override { return m_nsample; }

  // Return a sample.
  Code code(SampleIndex isam) const override { return data()[isam]; }

  // Return the voltage for a sample.
  double vin(Index isam) const override { return m_dvds*isam; }

  // Sampling frequency (Hz).
  double samplingFrequency() const override { return m_samplingFrequency; }

public:  // Table info

  // There are nvin() input voltage bins. Bin ivin starts at
  //   vinmin() + dvin()*ivin  mV
  Index nvin() const override { return m_nvin; }
  double dvin() const override { return m_dvin; }
  double vinmin() const override { return m_vinmin; }
  double vinmax() const override { return m_vinmin + nvin()*dvin(); }

  // Count table.
  // The # samples for ADC code iadc and input voltage bin ivin is count[iadc][ivin];
  const CountTable& countTable() const override { return m_table; }

  // Return the low edge of an input voltage bin.
  double vinLow(Index ivin) const override;

  // Return the center of an input voltage bin.
  double vinCenter(Index ivin) const override;

public:  // Methods specific to this class

  // Read (or reread) the data for this channel.
  int read();

  // Waveform.
  const CodeVector& data() const { return m_data; }

  // Rate at which voltage changes [mV/sec]
  double dvdt() const { return m_dvdt; }

  // Rate at which voltage changes per sample [mV]
  double dvds() const { return m_dvds; }

  // Nominal gain.
  double nomVinPerAdc() const { return m_nomVinPerAdc; }

private:

  Name m_topdir;
  Name m_sample;
  Name m_dataset;
  Index m_chip;
  Name m_chipLabel;
  Index m_chan;
  Code m_nadc;
  Index m_maxSample;
  SampleIndex m_nsample;
  Index m_nvin;
  double m_dvin;
  double m_vinmin;
  double m_samplingFrequency;
  double m_dvdt;
  double m_dvds;
  double m_nomVinPerAdc;

  CountTable m_table;
  CodeVector m_data;

};

#endif
