// AdcBinRecorder.cxx

#include "AdcBinRecorder.h"
#include <string>
#include <iostream>

using std::string;
using std::cout;
using std::endl;

//**********************************************************************
// Subclass methods.
//**********************************************************************

AdcBinRecorder::Peak::Peak(const SampleVector& sams) {
  if ( sams.size() == 0 ) return;
  size = sams.size();
  low = sams.front();
  high = sams.back();
  double sumOff = 0.0;
  for ( SampleIndex isam : sams ) {
    SampleIndex ioff = isam - low;
    sumOff += ioff;
  }
  meanOffset = sumOff/size;
  truncMean = low + SampleIndex(meanOffset);
}

//**********************************************************************
// Class methods.
//**********************************************************************

AdcBinRecorder::AdcBinRecorder(AdcCode a_code)
: m_code(a_code), m_minGap(0) { }

//**********************************************************************

int AdcBinRecorder::addSample(SampleIndex isam) {
  string myname = "AdcBinRecorder::addSample: ";
  if ( m_samples.size() && isam <= m_samples.back() ) {
    cout << myname << "Sample is out of order. Skipping." << endl;
    return 1;
  }
  m_samples.push_back(isam);
  m_minGap = 0.0;
  m_peaks.clear();
  return 0;
}

//**********************************************************************

int AdcBinRecorder::findPeaks(SampleIndex a_minGap) {
  m_minGap = a_minGap;
  m_peaks.clear();
  SampleVector sams;
  for ( SampleIndex isam : samples() ) {
    if ( sams.size() && isam - sams.back() > minGap() ) {
      m_peaks.emplace_back(sams);
    } else {
      sams.push_back(isam);
    }
  }
  if ( sams.size() ) m_peaks.emplace_back(sams);
  return m_peaks.size();
}

//**********************************************************************

const AdcBinRecorder::Peak& AdcBinRecorder::peak(Index ipeak) const {
  static Peak badPeak;
  if ( ipeak > npeak() ) return badPeak;
  return m_peaks[ipeak];
}

//**********************************************************************
