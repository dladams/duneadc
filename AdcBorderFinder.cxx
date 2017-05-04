// AdcBorderFinder.cxx

#include "AdcBorderFinder.h"

using Name = AdcBorderFinder::Name;

//**********************************************************************
// Subclass definitions.
//**********************************************************************

void AdcBorderFinder::SampleRange::print() const {
  cout << "start=" << begin() << ", size=" << size() << ", state="
       << stateName(state()) << endl;
}

//**********************************************************************
// Class definitions.
//**********************************************************************

Name AdcBorderFinder::stateName(State state) {
  if ( state == UNKNOWN ) return "UNKNOWN";
  if ( state == UNDER ) return "UNDER";
  if ( state == RISING ) return "RISING";
  if ( state == OVER ) return "OVER";
  if ( state == FALLING ) return "FALLING";
  return "StateError";
}

//**********************************************************************

AdcBorderFinder::
AdcBorderFinder(SampleIndex fenceWidth,
                SampleValue minThresh, SampleValue maxThresh,
                SampleValue minLimit, SampleValue maxLimit)
: m_fenceWidth(fenceWidth),
  m_minThresh(minThresh),
  m_maxThresh(maxThresh),
  m_minLimit(minLimit),
  m_maxLimit(maxLimit) { }

//**********************************************************************

int AdcBorderFinder::find(const AdcSampleReader& reader, SampleRangeVector& borders) const {
  const Name myname = "AdcBorderFinder::find: ";
  borders.clear();
  bool foundStartingFence = false; // Flag indicating initial block of good samples is found.
  State state = UNKNOWN;      // Current stream state.
  SampleIndex nsamFence = 0;  // # samples in the curent fence
  SampleRange range;          // Current range
  for ( SampleIndex isam=0; isam<reader.nsample(); ++isam ) {
    SampleValue code = reader.code(isam);
    bool sampleIsUnderThresh = code <= minThresh();
    bool sampleIsOverThresh = code >=maxThresh();
    bool sampleIsFence = !sampleIsUnderThresh && !sampleIsOverThresh;
    bool streamIsUnder = state == UNDER;
    bool streamIsOver = state == OVER;
    // Still looking for starting fence.
    if ( ! foundStartingFence ) {
      if ( sampleIsFence ) {
        ++nsamFence;
        if ( nsamFence >= fenceWidth() ) {
          SampleRange tmprange(isam+1-fenceWidth(), isam+1);
          cout << myname << "Found starting fence: ";
          tmprange.print();
          cout << endl;
          foundStartingFence = true;
        }
      } else {
        nsamFence = 0;
      }
    // Stream is inside a border plus incomplete trailing fence.
    // We are looking for the trailing fence.
    // Add range for border once that fence is found.
    } else if ( streamIsUnder || streamIsOver ) {
      if ( sampleIsFence ) {
        ++nsamFence;
        if ( nsamFence >= fenceWidth() ) {
          cout << myname << "Ending " << stateName(state) << " at " << isam << endl;
          borders.push_back(range);
          range.print();
          borders.back().print();
          range.reset();
          if ( state == UNDER ) state = RISING;
          if ( state ==  OVER ) state = FALLING;
        }
      } else {
        if ( streamIsUnder && code > minLimit() ) return 7;
        if ( streamIsOver && code < maxLimit() ) return 8;
        nsamFence = 0;
        range.setEnd(isam + 1);
      }
    // Stream is in a fence. Look for the next border flagged by one sample
    // outside of fence threholds.
    } else {
      if ( sampleIsUnderThresh ) {
        if ( nsamFence < fenceWidth() ) return 9;
        cout << myname << "Starting min border at " << isam << endl;
        range.set(isam, isam+1, UNDER);
        range.print();
        state = UNDER;
        nsamFence = 0;
      } else if ( sampleIsOverThresh ) {
        if ( nsamFence < fenceWidth() ) return 10;
        cout << myname << "Starting max border at " << isam << endl;
        range.set(isam, isam+1, OVER);
        range.print();
        state = OVER;
        nsamFence = 0;
      }
    }
  }
  return 0;
}

//**********************************************************************
