// AdcBinRecordBuilder.cxx

#include "AdcBinRecordBuilder.h"
#include <iostream>
using std::cout;
using std::endl;

//**********************************************************************

AdcBinRecordBuilder::AdcBinRecordBuilder(SampleIndex a_minGap)
: m_minGap(a_minGap), m_preader(nullptr) { }

//**********************************************************************

int AdcBinRecordBuilder::fill(const AdcSampleReader& reader) {
  const string myname = "AdcBinRecordBuilder::fill: ";
  const int dbg = 0;
  m_preader = &reader;
  m_abrs.clear();
  SampleValue nadc = reader.nadc();
  m_abrs.reserve(nadc);
  if ( dbg > 0 ) cout << myname << "Building records." << endl;
  for ( AdcCode code=0; code<nadc; ++code ) m_abrs.emplace_back(code, false);
  if ( dbg > 0 ) cout << myname << "Adding samples." << endl;
  for ( SampleIndex isam=0; isam<reader.nsample(); ++isam ) {
    AdcCode code = reader.mitigatedCode(isam);
    if ( code >= m_abrs.size() ) {
      cout << myname << "Error: ADC code " << code << " >= " << reader.nadc() << endl;
      abort();
    }
    if ( dbg > 1 ) cout << myname << "Adding sample " << isam << endl;
    m_abrs[code].addSample(isam);
  }
  if ( dbg > 0 ) cout << myname << "Finding peaks." << endl;
  for ( AdcBinRecord& abr : m_abrs ) abr.findPeaks(minGap());
  if ( dbg > 0 ) cout << myname << "Done." << endl;
  return 0;
};

//**********************************************************************
