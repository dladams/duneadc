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
                SampleValue minLimit, SampleValue maxLimit,
                bool useMitigated)
: m_fenceWidth(fenceWidth),
  m_minThresh(minThresh),
  m_maxThresh(maxThresh),
  m_minLimit(minLimit),
  m_maxLimit(maxLimit),
  m_useMitigated(useMitigated) { }

//**********************************************************************

int AdcBorderFinder::find(const AdcSampleReader& reader, SampleRangeVector& borders) const {
  const Name myname = "AdcBorderFinder::find: ";
  Index dbg = 1;
  if ( dbg ) {
    cout << myname << "Starting border search." << endl;
    cout << myname << "  fenceWidth = " << fenceWidth() << endl;
    cout << myname << "   minThresh = " << minThresh() << endl;
    cout << myname << "   maxThresh = " << maxThresh() << endl;
    cout << myname << "    minLimit = " << minLimit() << endl;
    cout << myname << "    maxLimit = " << maxLimit() << endl;
  }
  borders.clear();
  bool foundStartingFence = false; // Flag indicating initial block of good samples is found.
  State state = UNKNOWN;      // Current stream state.
  SampleIndex nsamFence = 0;  // # samples in the curent fence
  SampleRange range;          // Current range
  Index maxerr = 200;
  Index nerr = 0;
  for ( SampleIndex isam=0; isam<reader.nsample(); ++isam ) {
    //oct SampleValue code = reader.code(isam);
    SampleValue code = reader.mitigatedCode(isam);
    bool sampleIsUnderThresh = code <= minThresh();
    bool sampleIsOverThresh = code >= maxThresh();
    bool sampleIsFence = !sampleIsUnderThresh && !sampleIsOverThresh;
    bool streamIsUnder = state == UNDER;
    bool streamIsOver = state == OVER;
    // Still looking for starting fence.
    if ( ! foundStartingFence ) {
      if ( sampleIsFence ) {
        ++nsamFence;
        if ( nsamFence >= fenceWidth() ) {
          SampleRange tmprange(isam+1-fenceWidth(), isam+1);
          cout << myname << "Found starting fence at sample " << isam;
          //tmprange.print();
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
        if ( nsamFence >= fenceWidth()
             || (streamIsUnder && code > minLimit())
             || (streamIsOver && code < maxLimit()) ) {
          cout << myname << "Ending " << stateName(state)
               << " at ADC[" << isam << "] = " << code << endl;
          borders.push_back(range);
          //range.print();
          //borders.back().print();
          range.reset();
          if ( state == UNDER ) state = RISING;
          if ( state ==  OVER ) state = FALLING;
        }
      } else {
        //if ( nsamFence ) cout << myname << "Reset fence at ADC[" << isam << "] = " << code << endl;
        if ( streamIsUnder && code > minLimit() ) return 7;
        if ( streamIsOver && code < maxLimit() ) return 8;
        nsamFence = 0;
        range.setEnd(isam + 1);
      }
    // Stream is in a fence. Look for the next border flagged by one sample
    // outside of fence thresholds.
    } else {
      if ( sampleIsUnderThresh ) {
        if ( nerr < maxerr ) {
          cout << myname << "Starting min border at ADC[" << isam << "] = " << code << endl;
        }
        if ( nsamFence < fenceWidth() ) {
          if ( ++nerr < maxerr ) {
            cout << myname << "Too few samples in fence: " << nsamFence << " < " << fenceWidth()
                 << ". Consider a larger value for minLimit." << endl;
          }
          sampleIsFence = true;
          //return 9;
        } else {
          range.set(isam, isam+1, UNDER);
          //range.print();
          state = UNDER;
          nsamFence = 0;
        }
      } else if ( sampleIsOverThresh ) {
        if ( nerr < maxerr ) {
          cout << myname << "Starting max border at ADC[" << isam << "] = " << code << endl;
        }
        if ( nsamFence < fenceWidth() ) {
          if ( ++nerr < maxerr ) {
            cout << myname << "Too few samples in fence: " << nsamFence << " < " << fenceWidth()
                 << ". Consider a smaller value for maxLimit." << endl;
          }
          sampleIsFence = true;
          //return 10;
        } else {
          range.set(isam, isam+1, OVER);
          //range.print();
          state = OVER;
          nsamFence = 0;
        }
      }
      if ( sampleIsFence ) ++nsamFence;
    }
  }
  cout << myname << "Processed all samples." << endl;
  Index nrep = nerr > maxerr ? maxerr : nerr;
  if ( nerr ) cout << myname << "Reported " << nrep << " of " << nerr << " errors." << endl;
  if ( dbg ) cout << myname << "# borders found: " << borders.size() << endl;
  return 0;
}

//**********************************************************************
