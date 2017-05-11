// AdcBinarySampleReader.h
//
// Class to read binary data from an input stream.

#ifndef AdcBinarySampleReader_H
#define AdcBinarySampleReader_H

#include <string>
#include <iostream>
#include <vector>
#include "AdcTypes.h"
#include "AdcSampleReader.h"
#include "AdcBinRecord.h"

class SampleFunction;
class TTree;
class TCanvas;

class AdcBinarySampleReader : public AdcSampleReader {

public:

  using Name = std::string;

public:  // static members

  static SampleIndex badAdcCode() { return 1<<15; }

public:  // non-static members

  // Ctor from input file name.
  AdcBinarySampleReader(Name fname);

  // Ctor from input file name plus metadata.
  //      fname - file name (full path)
  //   a_sample - Sample name (e.g. 201703b_D02_6a)
  //       chip - chip index (within the dataset)
  //  chipLabel - global chip label
  //       icha - channel number
  //      fsamp - sampling frequency [Hz]
  AdcBinarySampleReader(Name fname, Name a_sample, Index chip, Name chipLabel, Index icha, double fsamp);

  // Dtor.
  ~AdcBinarySampleReader();

  // Sample name.
  Name sample() const override { return m_sample; }

  // Set the sample fuction that specifies vin.
  void setSampleFunction(const SampleFunction* samfun) { m_samfun = samfun; }

  // Underflow code.
  AdcCode underflowCode() const { return m_underflowCode; }

  // Overflow code.
  AdcCode overflowCode() const { return m_overflowCode; }

  // Mask to remove channel number.
  AdcCode chanMask() const { return m_chanMask; }

  // Shift to extract channel number.
  Index chanShift() const { return m_chanShift; }

  // File/stream name.
  Name fileName() { return m_fname; }

  // Input stream.
  std::istream* inputStream() { return m_pin; }

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

  // Read the data and fill data vector or tree if either of those flags is set.
  //   m_doTree - Fill the tree
  //   m_doData - Fill the data vactor
  // If both flags are false only # samples is filled.
  // If pdat is provided, that vector is cleared and filled with the ADC data.
  // Returns the tree.
  int read() const;

  // Return the waveform.
  AdcCode code(SampleIndex isam) const override;

  // The input voltage (mV) for sample isam.
  double vin(SampleIndex isam) const override;


  // Return the tree. Non-const builds tree if needed.
  TTree* tree();
  TTree* tree() const { return m_ptree; }

  // Draw the data. First creates a tree.
  TCanvas* draw();

public:  // Flags

  SampleIndex m_maxSample = 0;  // maximum # of samples to read (0 = all)
  SampleIndex m_nDump = 0;      // # of samples to display when reading file (0 = none)
  bool m_doTree = false;
  mutable bool m_doData = true;

private:

  Name m_sample;
  Name m_dsname;
  Index m_chip = badChip();
  Name m_chipLabel;
  mutable Index m_channel = badChannel();
  double m_fsamp = 0.0;
  AdcCode m_underflowCode;
  AdcCode m_overflowCode;
  AdcCode m_chanMask;
  Index m_chanShift;
  bool m_ownStream;
  std::istream* m_pin;
  Name m_fname;
  mutable bool m_haveReadFile;
  mutable SampleIndex m_nsample;
  mutable TTree* m_ptree;
  mutable SampleVector m_data;
  const SampleFunction* m_samfun = nullptr;

};

#pragma link C++ class AdcBinarySampleReader::SampleRange;
#pragma link C++ class AdcBinarySampleReader::SampleRangeVector;

#endif
