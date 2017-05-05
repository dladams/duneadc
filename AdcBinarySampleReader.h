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

class TTree;
class TCanvas;

class AdcBinarySampleReader : public AdcSampleReader {

public:

  using Name = std::string;

public:  // static members

  static SampleIndex badAdcCode() { return 1<<15; }

public:  // non-static members

  // Ctor from input file name.
  AdcBinarySampleReader(Name fname, SampleIndex afence);

  // Dtor.
  ~AdcBinarySampleReader();

  // Underflow code.
  AdcCode underflowCode() const { return m_underflowCode; }

  // Overflow code.
  AdcCode overflowCode() const { return m_overflowCode; }

  // Mask to remove channel number.
  AdcCode chanMask() const { return m_chanMask; }

  // Shift to extract channel number.
  Index chanShift() const { return m_chanShift; }

  // Fence for determining under and overflow regions.
  // There must be at least this many non-underflow/overflow
  // samples on either side of the region.
  SampleIndex fence() const { return m_fence; }

  // File/stream name.
  Name fileName() { return m_fname; }

  // Input stream.
  std::istream* inputStream() { return m_pin; }

  // Return the number of ADC bins.
  SampleValue nadc() const override { return 4096; }

  // Return the number of samples.
  SampleIndex nsample() const override;

  // Return the channel number retrieved from the high bits of the codes.
  Index channel() const override { return m_channel; }

  // Read the data and fill data vector or tree if either of those flags is set.
  //   m_doTree - Fill the tree
  //   m_doData - Fill the data vactor
  // If both flags are false only # samples is filled.
  // If pdat is provided, that vector is cleared and filled with the ADC data.
  // Returns the tree.
  int read() const;

  // Return the waveform.
  AdcCode code(SampleIndex isam) const override;

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

  AdcCode m_underflowCode;
  AdcCode m_overflowCode;
  AdcCode m_chanMask;
  Index m_chanShift;
  bool m_ownStream;
  std::istream* m_pin;
  Name m_fname;
  SampleIndex m_fence;
  mutable bool m_haveReadFile;
  mutable SampleIndex m_nsample;
  mutable Index m_channel;
  mutable TTree* m_ptree;
  mutable SampleVector m_data;

};

#pragma link C++ class AdcBinarySampleReader::SampleRange;
#pragma link C++ class AdcBinarySampleReader::SampleRangeVector;

#endif
