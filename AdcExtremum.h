// AdcExtremum.h
//
// Class to describe extrema of an Adc Waveform.
//
// An extremum consists for a tick index and a flg indicating
// if it is a minimum or maximum.

#ifndef AdcExtremum_H
#define AdcExtremum_H

#include <string>
#include <vector>
#include "AdcBinRecorder.h"

class AdcExtremum {

public:

  using SampleIndex = unsigned long;

  // Default ctor.
  AdcExtremum() = default;

  // Ctor from data.
  AdcExtremum(SampleIndex a_tick, bool a_isMin) : m_tick(a_tick), m_isMin(a_isMin) { }

  // Return the tick.
  SampleIndex tick() const { return m_tick; }

  // Return if this is a minimum or maximum.
  bool isMin() const { return m_isMin; }
  bool isMax() const { return !m_isMin; }

private:

  SampleIndex m_tick;
  bool m_isMin;

};

#endif
