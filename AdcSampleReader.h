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

  static Index badChannel() { return -1; }
  static Index badChip() { return -1; }

public:  // General

  // Dtor.
  virtual ~AdcSampleReader() =default;

  // Set the channel number.
  // Returns nonzero for error.
  virtual int setChannel(Index icha) =0;

  // Sample name.
  virtual Name sample() const =0;

  // Dataset name.
  // Derived from sample name and typically excludes the chip number.
  virtual Name dataset() const =0;

  // Chip number.
  virtual Index chip() const =0;

  // Channel number on the chip.
  virtual Index channel() const =0;

  // Number of channels on the chip
  virtual Index nchannel() const =0;

  // Number of ADC codes (4096 for the typical 12-bits)
  virtual Code nadc() const =0;

public:  // For waveforms

  // The total number of samples.
  virtual Index nsample() const =0;

  // Waveform. The ADC bin for tick isam.
  virtual Code code(Index isam) const =0;

  // The input voltage (mV) for sample isam.
  virtual double vin(Index isam) const =0;

  // Return the sampling frequency.
  virtual double samplingFrequency() const { return 0.0; }

  // Return a histogram of the waveform from sample idat to idat+ndat.
  // Every nshow'th point is shown.
  virtual TH1* histdata(unsigned int idat =0, unsigned int ndat =0, unsigned int nshow =1);

public:  // Table description.

  // There are nvin() input voltage bins. Bin ivin starts at
  //   vinmin() + dvin()*ivin  mV
  virtual Index nvin() const =0;
  virtual double dvin() const =0;
  virtual double vinmin() const =0;
  virtual double vinmax() const { return vinmin() + nvin()*dvin(); }

  // Count table.
  // The # samples for ADC code iadc and input voltage bin ivin is count[iadc][ivin];
  virtual const CountTable& countTable() const =0;

  // Return the low edge of an input voltage bin.
  virtual double vinLow(Index ivin) const;

  // Return the center of an input voltage bin.
  virtual double vinCenter(Index ivin) const;

};

#endif
