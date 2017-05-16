// AdcSampleReader.h
//
// David Adams
// April 2017
//
// Interface for classes that read ADC sample data.

#ifndef AdcSampleReader_H
#define AdcSampleReader_H

#include <string>
#include <vector>
#include "AdcTypes.h"
class TH1;
class TF1;

class AdcSampleReader {

public:

  using Name = std::string;
  using Index = unsigned int;
  using SampleIndex = unsigned long;
  using Count = unsigned int;
  using CountVector = std::vector<Count>;
  using CountTable = std::vector<CountVector>;

  using Code = unsigned short;
  using CodeVector = std::vector<Code>;

public:  // General

  // Dtor.
  virtual ~AdcSampleReader() =default;

  // Set the channel number and read data for that channel.
  // Returns nonzero for error e.g. this sample does not hhave data for that channel.
  virtual int setChannel(Index icha) {return 1;}

  // Sample name.
  virtual Name sample() const { return ""; }

  // Dataset name.
  // Derived from sample name and typically excludes the chip number.
  virtual Name dataset() const { return ""; }

  // Chip number and label.
  // The chip index should be unique within the dataset.
  // The chip label should uniquely identify the chip.
  virtual Index chip() const { return badChip(); }
  virtual Name chipLabel() const { return ""; }

  // Channel number on the chip.
  virtual Index channel() const { return badChannel(); }

  // Number of channels on the chip
  virtual Index nchannel() const { return 0; }

  // Number of ADC codes (4096 for the typical 12-bits)
  virtual Code nadc() const { return 0; }

  // Time data was taken in unix sec.
  virtual AdcTime time() const { return 0; }

public:  // For waveforms

  // Maximum # samples to read. Zero for all.
  // If this is nonzero, some data may not be read.
  virtual Index maxSample() const { return 0; }

  // The total number of samples read.
  virtual SampleIndex nsample() const =0;

  // Waveform. The ADC bin for tick isam.
  virtual Code code(SampleIndex isam) const =0;

  // The input voltage (mV) for sample isam.
  virtual double vin(SampleIndex isam) const { return 0.0; }
  double vinForTF1(double* x, double*) const;
  TF1* vinTF1(Name name = "sampleVin") const;

  // Return the sampling frequency.
  virtual double samplingFrequency() const { return 0.0; }

  // Return a histogram of the waveform from sample idat to idat+ndat.
  // Every nshow'th point is shown.
  // If histtime, then plot is vs time instead of tick.
  // If nshow < 0, then data is binned and error bars reflect range.
  virtual TH1* histdata(SampleIndex idat =0, SampleIndex ndat =0,
                        int nshow =1, bool histtime = false);

  // Return a histogram of the input voltage from sample idat to idat+ndat.
  // Every nshow'th point is shown.
  // If histtime, then ploat is vs time instead of tick.
  virtual TH1* histvin(SampleIndex idat =0, SampleIndex ndat =0,
                       unsigned int nshow =1, bool histtime = false);

public:  // Table description.

  // There are nvin() input voltage bins. Bin ivin starts at
  //   vinmin() + dvin()*ivin  mV
  Index nvin() const { return m_nvin; }
  double dvin() const { return m_dvin; }
  double vinmin() const { return m_vinmin; }
  double vinmax() const { return vinmin() + nvin()*dvin(); }

  // Count table.
  // The # samples for ADC code iadc and input voltage bin ivin is count[iadc][ivin];
  const CountTable& countTable() const { return m_table; }

  // Return the low edge of an input voltage bin.
  double vinLow(Index ivin) const;

  // Return the center of an input voltage bin.
  double vinCenter(Index ivin) const;

  // Build table from waveform.
  // Table will have nvin voltage bins width dvin starting at vinmin.
  int buildTableFromWaveform(Index nvin, double dvin, double vinmin);

protected:  // data: subclasses should fill this

  // Data describing the table.
  Index m_nvin = 0;
  double m_dvin = 0.0;
  double m_vinmin = 0.0;
  CountTable m_table;

};

#endif
