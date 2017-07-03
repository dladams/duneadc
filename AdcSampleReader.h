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
#include "AdcCodeMitigator.h"

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

  using AdcCodeVector = std::vector<AdcCode>;
  using AdcCodeMitigatorVector = std::vector<const AdcCodeMitigator*>;

public:  // General

  // Dtor.
  virtual ~AdcSampleReader();

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
  virtual AdcCode nadc() const { return 0; }

  // Time data was taken in unix sec.
  virtual AdcTime time() const { return 0; }

public:  // For waveforms

  // Maximum # samples to read. Zero for all.
  // If this is nonzero, some data may not be read.
  virtual Index maxSample() const { return 0; }

  // The total number of samples read.
  virtual SampleIndex nsample() const =0;

  // Waveform. The ADC bin for tick isam.
  virtual AdcCode code(SampleIndex isam) const =0;

  // The input voltage (mV) for sample isam.
  // Implementer *may* return dVin/dtick in *pdvds.
  virtual double vin(SampleIndex isam, double* pdvds =nullptr) const { return 0.0; }
  double vinForTF1(double* x, double*) const;
  TF1* vinTF1(Name name = "sampleVin") const;

  // Return the sampling frequency.
  virtual double samplingFrequency() const { return 0.0; }

  // Return a histogram of the waveform from sample idat to idat+ndat.
  // Every nshow'th point is shown.
  // If histtime, then plot is vs time instead of tick.
  // If nshow < 0, then data is binned and error bars reflect range.
  // if mitigated, the mitigated waveform is returned.
  // Caller is reponsible for manging the returned histogram.
  virtual TH1* histdata(SampleIndex idat =0, SampleIndex ndat =0,
                        int nshow =1, bool histtime = false,
                        bool mitigated =false) const;

  // Return a histogram of the input voltage from sample idat to idat+ndat.
  // Every nshow'th point is shown.
  // If histtime, then ploat is vs time instead of tick.
  // Caller is reponsible for manging the returned histogram.
  virtual TH1* histvin(SampleIndex idat =0, SampleIndex ndat =0,
                       unsigned int nshow =1, bool histtime = false) const;
 
public:  // Table description.

  // There are nvin() input voltage bins. Bin ivin starts at
  //   vinmin() + dvin()*ivin  mV
  Index nvin() const { return m_nvin; }
  double dvin() const { return m_dvin; }
  double vinmin() const { return m_vinmin; }
  double vinmax() const { return vinmin() + nvin()*dvin(); }

  // Count tables.
  // The # samples for ADC code iadc and input voltage bin ivin is count[iadc][ivin];
  const CountTable& countTable() const { return m_table; }
  const CountTable& countTableU() const { return m_tableu; }
  const CountTable& countTableD() const { return m_tabled; }

  // Return the low edge of an input voltage bin.
  double vinLow(Index ivin) const;

  // Return the center of an input voltage bin.
  double vinCenter(Index ivin) const;

  // If true, this analyzer holds separate tables for upward and downward sloping
  // data. Data for dVin/ds > 0 is in countTableU() and < 0 in countTableD().
  bool haveVinSlopeTables() const { return m_haveVinSlopeTables; }

  // Build table from waveform.
  // Table will have nvin voltage bins width dvin starting at vinmin.
  int buildTableFromWaveform(Index nvin, double dvin, double vinmin,
                             bool doAllTable =true, bool doSlopeTables =false);

public: // Mitigation.

  // Add ADC code mitigator.
  // This reader manages (deletes) its mitigators.
  void addMitigator(const AdcCodeMitigator* pmit);

  // Return the mitigated code for a sample.
  AdcCode mitigatedCode(SampleIndex isam) const;

  // Return the vector of mitigators.
  const AdcCodeMitigatorVector& mitigators() const { return m_mits; }

protected:  // data: subclasses should fill this

  // Vin-ADC tables
  Index m_nvin = 0;
  double m_dvin = 0.0;
  double m_vinmin = 0.0;
  CountTable m_table;
  CountTable m_tableu;
  CountTable m_tabled;
  bool m_haveVinSlopeTables =false;
  AdcCodeMitigatorVector m_mits;

};

#endif
