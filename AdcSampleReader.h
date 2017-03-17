// AdcSampleReader.cxx
//
// David Adams
// February 2017
//
// Class to read ADC sample data into memory.
// The sample is specified by a name and channel number.
//
// The data is summarized in a table giving the number of samples for each
// input voltage and output ADC code.
//
// Where the input data is in that format, the data here also includes the
// raw waveform, i.e. the ADC code for each tick, along with a paramterization
// giving the input voltage for that tick.

#ifndef AdcSampleReader_H
#define AdcSampleReader_H

#include "FileDirectory.h"
#include <string>
class TH1;

class AdcSampleReader {

public:

  using Name = std::string;
  using Index = unsigned int;
  using Count = unsigned int;
  using CountVector = std::vector<Count>;
  using CountTable = std::vector<CountVector>;

  using Code = unsigned short;
  using CodeVector = std::vector<Code>;

public:

  // Read in the data.
  //  asample - sample name
  //  achan - channel #
  //  maxsam - maximum # samples to read from a waveform (0 for all)
  AdcSampleReader(Name asample, Index achan, Index maxsam =0);

  // Sample name.
  Name sample() const { return m_sample; }

  // Dataset name.
  // Derived from sample name and typically excludes the chip number.
  Name dataset() const { return m_dataset; }

  // Chip number.
  Index chip() const { return m_chip; }

  // Channel number on the chip.
  Index channel() const { return m_chan; }

  // Number of ADC codes (4096 for the typical 12-bits)
  Code nadc() const { return m_nadc; }

  // Sampling frequency (Hz).
  double samplingFrequency() const { return m_samplingFrequency; }

  // Rate at which voltage changes [mV/sec]
  double dvdt() const { return m_dvdt; }

  // Nominal gain.
  double nomVinPerAdc() const { return m_nomVinPerAdc; }

  // There are nvin() input voltage bins. Bin ivin starts at
  //   vinmin() + dvin()*ivin  mV
  Index nvin() const { return m_nvin; }
  double dvin() const { return m_dvin; }
  double vinmin() const { return m_vinmin; }
  double vinmax() const { return m_vinmin + nvin()*dvin(); }

  // The total number of samples.
  // Same as data.size() for a waveform.
  Index nsample() const { return m_nsample; }

  // Count table.
  // The # samples for ADC code iadc and input voltage bin ivin is count[iadc][ivin];
  const CountTable& countTable() const { return m_table; }

  // Waveform.
  const CodeVector& data() const { return m_data; }

  // Return the low edge of an input voltage bin.
  double vinLow(Index ivin) const;

  // Return the center of an input voltage bin.
  double vinCenter(Index ivin) const;

  // Return a histogram of the waveform from sample idat to idat+ndat.
  // Every nshow'th point is shown.
  TH1* histdata(unsigned int idat =0, unsigned int ndat =0, unsigned int nshow =1);

private:

  Name m_topdir;
  Name m_sample;
  Name m_dataset;
  Index m_chip;
  Index m_chan;
  Code m_nadc;
  Index m_nvin;
  double m_dvin;
  double m_vinmin;
  double m_samplingFrequency;
  double m_dvdt;
  double m_nomVinPerAdc;
  Index m_nsample;

  CountTable m_table;
  CodeVector m_data;

};

#endif
