// AdcBinRecorder
//
// David Adams
// March 2017
//
// Class to record the samples for an ADC bin.

#ifndef AdcBinRecorder_H
#define AdcBinRecorder_H

class AdcBinRecorder {

public:

  using AdcCode = unsigned short;
  using Index = unsigned int;
  using SampleIndex = unsigned long;
  using SampleVector = std::vector<SampleIndex>;

  // Peak holds the description of one peak.
  struct Peak {
    Peak() = default;
    Peak(const SampleVector& sams);
    SampleIndex size = 0;
    SampleIndex low =0;
    SampleIndex high =0;
    double meanOffset =0.0;    // mean = low + meanOffset
    SampleIndex truncMean =0;  // mean truncated to sample index
  };

  using PeakVector = std::vector<Peak>;

  // Ctor.
  // a_code - ADC code for this bin.
  AdcBinRecorder(AdcCode a_code =0);

  // Add a sample to the bin.
  // Samples are rejected if they are not in order.
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

  // Return the samples for this bin.
  const SampleVector& samples() { return m_samples; }

  // Return the number of peaks and the peaks.
  Index npeak() const { return m_peaks.size(); };
  const Peak& peak(Index ipeak) const;
  const PeakVector& peaks() { return m_peaks; }

private:

  AdcCode m_code;
  SampleVector m_samples;
  SampleIndex m_minGap;
  PeakVector m_peaks;
  
};

#endif
