// AdcBinRecordBuilder.h

// David Adams
// May 2017

// Construct bin records for a reader and find peaks in those
// those records.

#ifndef AdcBinRecordBuilder_H
#define AdcBinRecordBuilder_H

#include "AdcBinRecord.h"
#include "AdcSampleReader.h"

class AdcBinRecordBuilder {

public:

  // Ctor.
  // minGap - The minimum gap between peaks.
  AdcBinRecordBuilder(SampleIndex minGap);

  // Fill records.
  int fill(const AdcSampleReader& reader);

  // Configuration getters.
  SampleIndex minGap() const { return m_minGap; }

  // Return the reader used for the data here.
  const AdcSampleReader* reader() const { return m_preader; }

  // Return the bin records. There is one for each ADC bin.
  const AdcBinRecordVector& binRecords() const { return m_abrs; }

private:

  SampleIndex m_minGap;
  const AdcSampleReader* m_preader;
  AdcBinRecordVector m_abrs;

};

#endif
