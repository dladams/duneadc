// AdcBinRecord
//
// David Adams
// March 2017
//
// Class to record the samples for an ADC bin.

#ifndef AdcBinRecord_H
#define AdcBinRecord_H

#include <string>
#include "AdcTypes.h"

class TH1;

class AdcBinRecord {

public:

  using Name = std::string;

  // Peak holds the description of one peak.
  struct Peak {
    Peak() = default;
    Peak(const SampleIndexVector& sams);
    SampleIndex size = 0;
    SampleIndex low =0;
    SampleIndex high =0;
    double meanOffset =0.0;    // mean = low + meanOffset
    SampleIndex truncMean =0;  // mean truncated to sample index
    double rms = 0.0;          // RMS (from mean) of the distribution
    double mean() const { return low + meanOffset; }
    SampleIndex width() const { return high - low; }
  };

  using PeakVector = std::vector<Peak>;
  using HistVector = std::vector<TH1*>;

  // Ctor from ADC bin (code).
  //   a_code - ADC code for this bin.
  //   doHist - If true a histogram is filled for each peak.
  AdcBinRecord(AdcCode a_code =0, bool doHist =false);

  // Ctor from string label.
  //   slab - Label for this bin.
  //   doHist - If true a histogram is filled for each peak.
  AdcBinRecord(Name slab, bool doHist =false);

  // Dtor. Deletes histos.
  ~AdcBinRecord();

  // Add a sample index (tick) to the bin.
  // Samples must be added in order.
  // Returns nonzero for this or other error.
  int addSample(SampleIndex isam);

  // Find the peaks for this bin.
  //   minGap - peaks are separated by at least this many samples
  // Returns the number of peaks or < 0 for error.
  int findPeaks(SampleIndex minGap);

  // Return the ADC code.
  AdcCode code() const { return m_code; }

  // Return the gap used for peak finding.
  SampleIndex minGap() const { return m_minGap; }

  // Return the ticks for this bin.
  const SampleIndexVector& samples() const { return m_samples; }

  // Return the number of peaks and the peaks.
  Index npeak() const { return m_peaks.size(); };
  const Peak& peak(Index ipeak) const;
  const PeakVector& peaks() const { return m_peaks; }

  // Return the peak histograms.
  const HistVector& peakHists() const { return m_peakHists; }

private:

  AdcCode m_code;
  Name m_slab;
  bool m_doHist;
  SampleIndexVector m_samples;
  SampleIndex m_minGap;
  PeakVector m_peaks;
  HistVector m_peakHists;
  
};

using AdcBinRecordVector = std::vector<AdcBinRecord>;
#pragma link C++ class AdcBinRecordVector;

#endif
