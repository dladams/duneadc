// AdcFembTreeSampleReader.h
//
// Class to read binary data from an input stream.

#ifndef AdcFembTreeSampleReader_H
#define AdcFembTreeSampleReader_H

#include <string>
#include <iostream>
#include <vector>
#include "AdcTypes.h"
#include "AdcSampleReader.h"
#include "AdcBinRecord.h"

class SampleFunction;
class TFile;
class TTree;
class TCanvas;

class AdcFembTreeSampleReader : public AdcSampleReader {

public:  // non-static members

  // Ctor from input file name, channel, sample name and range of ticks (nsam==0 ==> all).
  // Sample name adds chipc, channel and time from metadata.
  AdcFembTreeSampleReader(Name fname, Index chan, Name ssam,
                          SampleIndex isam0 =0, SampleIndex nsam =0);

  // Dtor.
  ~AdcFembTreeSampleReader();

  // Sample name.
  Name sample() const override { return m_sample; }

  // Return the dataset name.
  Name dataset() const override { return m_dsname; }

  // Return the chip index.
  Index chip() const override { return m_chip; }

  // Return the chip label.
  Name chipLabel() const override { return m_chipLabel; }

  // Return the channel number.
  // From metadata or high bits of the packed data.
  Index channel() const override { return m_channel; }

  // Return the number of ADC bins.
  SampleValue nadc() const override { return 4096; }

  // Return the number of ADC bins.
  Index nchannel() const override { return 16; }

  // Return the number of samples.
  SampleIndex nsample() const override;

  // Return the sampling frequency.
  double samplingFrequency() const override { return m_fsamp; }

  // Return the DAQ time in unix sec.
  AdcTime time() const override { return m_time; }

  // Return the waveform.
  AdcCode code(SampleIndex isam) const override;

  // The input voltage (mV) for sample isam.
  double vin(SampleIndex isam) const override;

  // Set the sample function that specifies vin.
  // If this is not called, function is taken from input tree.
  void setSampleFunction(const SampleFunction* samfun) { m_samfun = samfun; }

  // File/stream name.
  Name inputFileName() { return m_fname; }

  // Input file.
  TFile* inputFile() { return m_pinFile; }

  // Return the tree.
  TTree* inputTree() const { return m_pinTree; }

  // Underflow code.
  AdcCode underflowCode() const { return m_underflowCode; }

  // Overflow code.
  AdcCode overflowCode() const { return m_overflowCode; }

  // Metadata (in s, mV).
  long adcSerial() const { return m_adcSerial; }
  long feSerial() const { return m_feSerial; }
  long vinAmp() const { return m_vinAmp; }
  long vinOffset() const { return m_vinOffset; }
  long vinMin() const { return -vinAmp() + vinOffset(); }
  long vinMax() const { return vinAmp() + vinOffset(); }
  long vinFreq() const { return m_vinFreq; }

public:  // Flags

  SampleIndex m_maxSample = 0;  // maximum # of samples to read (0 = all)
  SampleIndex m_nDump = 0;      // # of samples to display when reading file (0 = none)
  bool m_doTree = false;
  mutable bool m_doData = true;

private:

  Name m_fname;
  Name m_sample;
  Name m_dsname;
  Index m_chip = badChip();
  Name m_chipLabel;
  Index m_channel = badChannel();
  double m_fsamp = 0.0;
  AdcTime m_time;
  SampleIndex m_isam0;
  SampleIndex m_nsam;
  TFile* m_pinFile;
  TTree* m_pinTree;
  TTree* m_pmdTree;
  std::vector<int>* m_pdata;
  std::vector<float>* m_pvin;
  TTree* m_poutTree;
  AdcCode m_underflowCode = 0;
  AdcCode m_overflowCode = 4095;
  const SampleFunction* m_samfun = nullptr;

  // Metadata.
  long m_adcSerial =0;
  long m_feSerial =0;
  unsigned long m_vinRate =0u;
  double m_vinAmp =0.0;
  double m_vinOffset =0.0;
  double m_vinFreq =0.0;

};

#pragma link C++ class AdcFembTreeSampleReader::SampleRange;
#pragma link C++ class AdcFembTreeSampleReader::SampleRangeVector;

#endif
