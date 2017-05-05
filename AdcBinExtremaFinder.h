// AdcBinExtremaFinder.h
//
// Tool to find ADC sample extrema using ADC bins.
// The midpoints between peaks in each bin are used as
// measurements of the extrema positions. Peaks in
// that distribution above some threshold constitute the
// extrema.

#ifndef AdcBinExtremaFinder_H
#define AdcBinExtremaFinder_H

#include "AdcTypes.h"
#include "AdcExtremaFinder.h"
#include "AdcSampleReader.h"
#include "AdcExtremum.h"

class TTree;
class TCanvas;

class AdcBinExtremaFinder : public AdcExtremaFinder {

public:

  // Ctor.
  // Ctor from configuration parameters.
  //   minGapBin - Minimum gap between peaks in the bins distributions.
  //   minGapExt - Minimum gap between peaks in the extremum distribution.
  //   nbinThresh - Minimum # bins for an extremum peak to define an extremum
  AdcBinExtremaFinder(SampleIndex a_minGapBin, SampleIndex a_minGapExt, Index a_nbinThresh);

  // Dtor.
   ~AdcBinExtremaFinder() =default;

  // Configuration getters.
  SampleIndex minGapBin() const { return m_minGapBin; }
  SampleIndex minGapExt() const { return m_minGapExt; }
  SampleIndex nbinThresh() const { return m_nbinThresh; }

  // Find all the extrema in a waveform.
  // The extremum vector is first cleared.
  int find(const AdcSampleReader& reader, AdcExtrema& ext) const override;

private:

  SampleIndex m_minGapBin;
  SampleIndex m_minGapExt;
  SampleIndex m_nbinThresh;

};

#endif
