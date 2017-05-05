// AdcBorderFinder.h
//
// Class to find  min and max ADC borders in an ADC sample.
//
// A fence is a contiguous region of a waveform with at least
// fenceWidth samples where the sample values are all above
// minThresh and below max thresh.
//
// A min border is a contiguous region of a waveform between
// two fences where all samples have ADC count below minLimit.
//
// A max border is a contiguous region of a waveform between
// two fences where all samples have ADC count below maxLimit.

#ifndef AdcBorderFinder_H
#define AdcBorderFinder_H

#include <string>
#include <iostream>
#include <vector>
#include "AdcSampleReader.h"

class AdcBorderFinder {

public:

  using Name = std::string;
  using SampleValue = unsigned short;
  using SampleIndex = unsigned long;
  using SampleVector = std::vector<short>;

  // State for sample ranges.
  enum State {UNKNOWN, UNDER, RISING, OVER, FALLING };

public: // Subclass describing a range of samples.

  class SampleRange {

  public:

    // Ctors.
    SampleRange(SampleIndex s1, SampleIndex s2 =0, State st =UNKNOWN)
    : range(s1, s2), m_state(st) { }
    SampleRange() : SampleRange(badSampleIndex(), 0) { }

    // Setters.
    void setBegin(SampleIndex sam) { range.first = sam; }
    void setEnd(SampleIndex sam) { range.second = sam; }
    void setState(State state) { m_state = state; }
    void set(SampleIndex sambeg, SampleIndex samend, State state) {
      setBegin(sambeg); setEnd(samend); setState(state);
    }
    void reset() { m_begin = badSampleIndex(); m_end = 0; m_state=UNKNOWN; }

    // Getters.
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
  static Name stateName(State state);

public:  // non-static members

  // Ctor from configuration parameters.
  AdcBorderFinder(SampleIndex fenceWidth,
                  SampleValue minThresh, SampleValue maxThresh,
                  SampleValue minLimit, SampleValue maxLimit);

  // Dtor.
  ~AdcBorderFinder() =default;

  // Configuration getters.
  SampleIndex fenceWidth() const { return m_fenceWidth; }
  SampleValue minThresh() const { return m_minThresh; }
  SampleValue maxThresh() const { return m_maxThresh; }
  SampleValue minLimit() const { return m_minLimit; }
  SampleValue maxLimit() const { return m_maxLimit; }

  // Find the borders in a waveform.
  // Return 0 for success.
  int find(const AdcSampleReader& reader, SampleRangeVector& borders) const;

private:  // Configuration data.

  SampleIndex m_fenceWidth;
  SampleValue m_minThresh;
  SampleValue m_maxThresh;
  SampleValue m_minLimit;
  SampleValue m_maxLimit;

};

//#pragma link C++ class AdcBorderFinder::SampleRange;
#pragma link C++ class AdcBorderFinder::SampleRangeVector;

#endif
