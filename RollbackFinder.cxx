// RollbackFinder.cxx

#include "RollbackFinder.h"
#include "AdcSampleReader.h"

//**********************************************************************

RollbackFinder::
RollbackFinder(const AdcSampleReader& a_rdr,
               SampleIndex a_maxRollback,
               AdcCode a_maxCode)
: m_rdr(a_rdr),
  m_maxRollback(a_maxRollback),
  m_maxCode(a_maxCode) {
  SampleIndex nsam = a_rdr.nsample();
  if ( nsam == 0 ) return;
  SampleIndex nsatHere = 0;   //  # saturated samples preceding the current sample
  SampleIndex nsatLast = 0;   //  # samples  in the last saturated region
  SampleIndex isatLast = nsam;   //  Last saturated sample.
  SampleIndex nunsHere = 0;  // # candidate unsaturated
  AdcCode code = a_rdr.mitigatedCode(0);
  SampleRegion region(nsam,0);
  bool inRegion = false;  // True if last sample is in a region of saturation.
  for ( SampleIndex isam=0; isam<a_rdr.nsample(); ++isam ) {
    code = a_rdr.mitigatedCode(isam);
    bool saturated = code >= m_maxCode;
    bool saveRegion = false;
    if ( inRegion ) {
      if ( saturated ) {
        nunsHere = 0;
        region.second = isam;
      } else {
        ++nunsHere;
        if ( nunsHere >= m_maxRollback ) {
          saveRegion = true;
          inRegion = false;
        }
      }
    } else {
      if ( saturated ) {
        region.first = isam;
        region.second = isam;
        inRegion = true;
      }
    }
    bool last = isam+1 == a_rdr.nsample();
    saveRegion |= last && inRegion;
    if ( saveRegion ) {
      m_regs.push_back(region);
      region.first = nsam;
      region.second = 0;
    }
  }
}

//**********************************************************************

AdcCode RollbackFinder::mitigatedCode(AdcCode code, SampleIndex isam) const {
  for ( SampleRegion reg : m_regs ) {
    if ( isam >= reg.first && isam <= reg.second ) return m_maxCode;
  }
  return code;
}

//**********************************************************************
