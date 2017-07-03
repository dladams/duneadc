// RollbackFinder.h

// David Adams
// July 2017
//
// This class removes rollback regions in an ADC waveform.
// Rollback is a region where input voltage above the
// maximum input voltage of the ADC returns other than
// the maximum count.
//
// Here rollback is identified by looking for a contiguous
// region of fewer than maxRollback ticks surrounded by ticks
// at maximum count.

#ifndef RollbackFinder_H
#define RollbackFinder_H

#include "AdcCodeMitigator.h"

#include <utility>
#include <vector>

class AdcSampleReader;

class RollbackFinder : public AdcCodeMitigator {

public:

  using SampleRegion = std::pair<SampleIndex, SampleIndex>;
  using SampleRegionVector = std::vector<SampleRegion>;

  // Ctor.
  RollbackFinder(const AdcSampleReader& rdr,
                 SampleIndex maxRollback,
                 AdcCode maxCode =4095);

  // Return waveform with rollback correction.
  AdcCode mitigatedCode(AdcCode code, SampleIndex isam) const override;

private:  // data

  const AdcSampleReader& m_rdr;
  SampleIndex m_maxRollback;
  AdcCode m_maxCode;
  SampleRegionVector m_regs;
  
};

#endif
