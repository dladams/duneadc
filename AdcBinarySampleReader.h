// AdcBinarySampleReader.h
//
// Class to read binary data from an input stream.

#ifndef AdcBinarySampleReader_H
#define AdcBinarySampleReader_H

#include <string>
#include <iostream>
#include <vector>

class TTree;
class TCanvas;

class AdcBinarySampleReader {

public:

  using Name = std::string;
  using AdcCode = unsigned short;
  using Index = unsigned int;
  using SampleIndex = unsigned long;

  // State for sample ranges.
  enum State {UNKNOWN, UNDER, RISING, OVER, FALLING };

public: // Subclass describing a range of samples.

  class SampleRange {
  public:
    SampleRange(SampleIndex s1, SampleIndex s2 =0, State st =UNKNOWN)
    : range(s1, s2), m_state(st) { }
    SampleRange() : SampleRange(badSampleIndex(), 0) { }
    void setBegin(SampleIndex sam) { range.first = sam; }
    void setEnd(SampleIndex sam) { range.second = sam; }
    void setState(State state) { m_state = state; }
    void set(SampleIndex sambeg, SampleIndex samend, State state) {
      setBegin(sambeg); setEnd(samend); setState(state);
    }
    void reset() { m_begin = badSampleIndex(); m_end = 0; m_state=UNKNOWN; }
    SampleIndex begin() const { return range.first; }
    SampleIndex end() const { return range.second; }
    State state() const { return m_state; }
    SampleIndex size() const { return end() > begin() ? end() - begin() : 0; }
    void print() const;
  private:
    SampleIndex m_begin;
    SampleIndex m_end; 
    std::pair<SampleIndex, SampleIndex> range;
    State m_state;
  };

  using SampleRangeVector = std::vector<SampleRange>;

public:  // static members

  static SampleIndex badSampleIndex() { return 1UL<<63; }
  static SampleIndex badAdcCode() { return 1<<15; }

  static string stateName(State state);

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

  // Shift extract channel number.
  Index chanShift() const { return m_chanShift; }

  // Fence for deteriming under and overflow regions.
  // There must be at least this many non-underflow/overflow
  // samples on either side of the region.
  SampleIndex fence() const { return m_fence; }

  // File/stream name.
  Name fileName() { return m_fname; }

  // Input stream.
  std::istream* inputStream() { return m_pin; }

  // Read the stream.
  int read();

  // Return the number of samples.
  SampleIndex nsample() const { return m_nsample; }

  // Return all the borders (under and overflow ranges).
  const SampleRangeVector& borders() const { return m_borders; }

  // Return the channel number retrieved from the high bits of the codes.
  Index channel() const { return m_channel; }

  // Return the data as a Root tree.
  TTree* tree();

  // Draw the data. First creates a tree.
  TCanvas* draw();

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
  SampleRangeVector m_borders;
  Index m_channel;
  TTree* m_ptree;

};

#pragma link C++ class AdcBinarySampleReader::SampleRange;
#pragma link C++ class AdcBinarySampleReader::SampleRangeVector;

#endif
