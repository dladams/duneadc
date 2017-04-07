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
  // Read in the data.
  //  asample - sample name, e.g. "201703a_D20"
  //  maxsam - maximum # samples to read from a waveform (0 for all)
  AdcTestSampleReader(Name asample, Index maxsam =0);

  // Dtor;
  ~AdcTestSampleReader() = default;

  // Set the channel and read the data for that channel.
  int setChannel(Index icha);

  // Sample name.
  Name sample() const { return m_sample; }

  // Dataset name.
  // Derived from sample name and typically excludes the chip number.
  Name dataset() const { return m_dataset; }

  // Chip number.
  Index chip() const { return m_chip; }

  // Channel number on the chip.
  Index channel() const { return m_chan; }

  // Channel number on the chip.
  Index nchannel() const { return 16; }

  // Number of ADC codes (4096 for the typical 12-bits)
  Code nadc() const { return m_nadc; }

public:  // Waveform info.

  // The total number of samples.
  // Same as data.size() for a waveform.
  Index nsample() const { return m_nsample; }

  // Return a sample.
  Code code(Index isam) const { return data()[isam]; }

  // Return the voltage for a sample.
  double vin(Index isam) const { return m_dvds*isam; }

  // Sampling frequency (Hz).
  double samplingFrequency() const { return m_samplingFrequency; }

public:  // Table info

  // There are nvin() input voltage bins. Bin ivin starts at
  //   vinmin() + dvin()*ivin  mV
  Index nvin() const { return m_nvin; }
  double dvin() const { return m_dvin; }
  double vinmin() const { return m_vinmin; }
  double vinmax() const { return m_vinmin + nvin()*dvin(); }

  // Count table.
  // The # samples for ADC code iadc and input voltage bin ivin is count[iadc][ivin];
  const CountTable& countTable() const { return m_table; }

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
  Index m_chan;
  Code m_nadc;
  Index m_nsample;
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
