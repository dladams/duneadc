// AdcBinarySampleReader.h
//
// Class to read binary data from an input stream.

#ifndef AdcBinarySampleReader_H
#define AdcBinarySampleReader_H

#include <string>
#include <iostream>
#include <vector>
#include "AdcSampleReader.h"
#include "AdcBinRecorder.h"

class TTree;
class TCanvas;

class AdcBinarySampleReader : public AdcSampleReader {

public:

  using Name = std::string;
  using AdcCode = unsigned short;
  using Index = unsigned int;
  using SampleIndex = unsigned long;
  using AdcCodeVector = std::vector<short>;
  using AdcBinRecorderVector = std::vector<AdcBinRecorder>;

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

  // Return the number of samples.
  SampleIndex nsample() const override { return m_nsample; }

  // Return the channel number retrieved from the high bits of the codes.
  Index channel() const override { return m_channel; }

  // Return the bin recorders. There is one for each ADC bin.
  const AdcBinRecorderVector& binRecorders() const { return m_abrs; }

  // Return the bin recorder holding the ADC bin peak averages.
  // These give the ticks about which the ADC counts are symmetric.
  const AdcBinRecorder& avgBinRecorder() const { return m_avgBins; }

  // Find the input voltage extrema, i.e. minima and maxima.
  // Looks for peaks in the ADC bin peak average distribution with
  // more than minSize entries (bins).
  class Extremum {
  public:
    Extremum() = default;
    Extremum(SampleIndex a_tick, bool a_isMin) : tick(a_tick), isMin(a_isMin) { }
    SampleIndex tick;
    bool isMin;
  };
  using Extrema = std::vector<Extremum>;
  Extrema findExtrema(Index minSize) const;

  // Read the data and fill the bin recorders or tree.
  //   doBins - Fill the bin recorders if not already filled
  //   doTree - Fill the tree if not already filled
  //   doData - Fill the data vactor
  // If all flags are false only # samples is filled.
  // If pdat is provided, that vector is cleared and filled with the ADC data.
  // Returns the tree.
  int read();

  // Waveform.
  AdcCode code(SampleIndex isam) const override;

  // Return the tree. Non-const builds tree if needed.
  TTree* tree();
  TTree* tree() const { return m_ptree; }

  // Draw the data. First creates a tree.
  TCanvas* draw();

public:  // Flags

  SampleIndex m_maxSample = 0;  // maximum # of samples to read (0 = all)
  SampleIndex m_nDump = 0;      // # of samples to display when reading file (0 = none)
  bool m_doBins = false;
  bool m_doTree = false;
  bool m_doData = true;

private:

  AdcCode m_underflowCode;
  AdcCode m_overflowCode;
  AdcCode m_chanMask;
  Index m_chanShift;
  bool m_ownStream;
  std::istream* m_pin;
  Name m_fname;
  SampleIndex m_fence;
  bool m_haveReadFile;
  SampleIndex m_nsample;
  AdcBinRecorderVector m_abrs;
  AdcBinRecorder m_avgBins;
  Index m_channel;
  TTree* m_ptree;
  AdcCodeVector m_data;

};

#pragma link C++ class AdcBinarySampleReader::SampleRange;
#pragma link C++ class AdcBinarySampleReader::SampleRangeVector;

#endif
