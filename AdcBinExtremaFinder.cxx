// AdcBinExtremaFinder.cxx

#include "AdcBinExtremaFinder.h"
#include <iostream>
#include <set>
#include "AdcBinRecordBuilder.h"

using std::string;
using std::cout;
using std::endl;
using std::multiset;

//**********************************************************************

AdcBinExtremaFinder::
AdcBinExtremaFinder(SampleIndex a_minGapBin, SampleIndex a_minGapExt, Index a_nbinThresh)
: m_minGapBin(a_minGapBin), m_minGapExt(a_minGapExt), m_nbinThresh(a_nbinThresh) { }

//**********************************************************************

int AdcBinExtremaFinder::find(const AdcSampleReader& reader, AdcExtrema& exts) const {
  const string myname = "AdcBinExtremaFinder::find: ";
  int dbg = 1;
  exts.clear();
  AdcBinRecordBuilder brb(minGapBin());
  if ( brb.fill(reader) ) return 1;
  // Loop over bins. For each bin, use the averages of adjacent peaks to define
  // candidate extrema.
  multiset<SampleIndex> candidateExtremaTicks;
  for ( const AdcBinRecord& abr : brb.binRecords() ) {
    // Record the average of each adjacent pair of peaks.
    if ( abr.npeak() > 1 ) {
      //cout << myname << "Adding candidates for bin " << abr.code() << endl;
      for ( Index ipea=0; ipea<abr.npeak()-1; ++ipea ) {
        SampleIndex avg = (abr.peak(ipea).truncMean + abr.peak(ipea+1).truncMean)/2;
        candidateExtremaTicks.insert(avg);
        //cout << myname << "  Tick " << avg << endl;
      }
    }
  }
  if ( dbg > 0 ) cout << myname << "# candidate entries: " << candidateExtremaTicks.size() << endl;
  AdcBinRecord candidateExtrema;
  // Find peaks in the average distribution.
  for ( SampleIndex extTick : candidateExtremaTicks ) {
    candidateExtrema.addSample(extTick);
  }
  candidateExtrema.findPeaks(minGapExt());
  if ( dbg > 0 ) cout << myname << "# candidate extrema: " << candidateExtrema.npeak() << endl;
  // Retain extrema with nbinThresh entries.
  for ( Index ipea=0; ipea<candidateExtrema.npeak(); ++ipea ) {
    const AdcBinRecord::Peak& peak = candidateExtrema.peak(ipea);
    if ( peak.size >= nbinThresh() ) {
      SampleIndex isam = peak.truncMean;
      AdcCode code = reader.code(isam);
      bool isMin = code < reader.nadc()/2;
      exts.emplace_back(isam, isMin);
    }
  }
  if ( dbg > 0 ) cout << myname << "# selected extrema: " << exts.size() << endl;
  return 0;
}
  
//**********************************************************************
