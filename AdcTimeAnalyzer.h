// AdcTimeAnalyzer.h

// David Adams
// May 2017
//
// Class for analyzing multiple time samples for a given ADC chip and channel.

#ifndef AdcTimeAnalyzer_H
#define AdcTimeAnalyzer_H

#include <string>
#include <memory>
#include "AdcTypes.h"

class TH1;
class AdcCalibrationTree;

class AdcTimeAnalyzer {

public:

  using Name = std::string;
  using AdcTimeAnalyzerPtr = std::unique_ptr<AdcTimeAnalyzer>;
  using AdcTimeAnalyzerVector = std::vector<AdcTimeAnalyzerPtr>;
  using HistPtr = std::unique_ptr<TH1>;
  using HistVector = std::vector<HistPtr>;

  // Return a vector of analyzers. Index is the channel number.
  static AdcTimeAnalyzerVector create(Name a_dsname, Index a_chip, Index nchan =16);

  // Ctor from tree name and chip and channel number.
  AdcTimeAnalyzer(Name a_dsname, Index a_chip, Index a_chan, int dbg=1);

  // Dtor.
  ~AdcTimeAnalyzer();

  // Display report.
  int report(bool verbose =true);

  // Return a histogram of the mean Vin (calibration) for all times for a given ADC bin.
  //   opt = 0 - raw mean
  //         1 - residual: mean - mean of linear fit
  //         2 - pedestal corrected: mean - (mean at 300 mV)
  TH1* histMean(Index iadc, int opt, bool draw =false, int dbg =0) const;

  // Return a histogram of the RMS for all times for a given ADC bin.
  TH1* histRms(Index iadc, bool draw =false, int dbg =0) const;

  // Return a histogram of the RMS vs. time index for a given ADC bin.
  TH1* histRmsTime(Index iadc, bool draw =false) const;

  // Return a histogram of the # sample counts for a given ADC bin.
  TH1* histCount(Index iadc, bool draw =false, int dbg =0) const;

  // Return a histogram of the # sample counts vs. time index for a given ADC bin.
  TH1* histCountTime(Index iadc, bool draw =false) const;

  // Return a histogram of the RMS of the mean ADC distribution.
  // There is one entry for each ADC bin.
  TH1* histMeanRms(int opt, bool draw =false, int dbg =0);

  // For a given bin, return the number of good, bad and low-stat samples.
  Index nGoodSamples(Index iadc) const;
  Index nBadSamples(Index iadc) const;
  Index nLowStatSamples(Index iadc) const;

  Name dsname;
  Index chip;
  Index chan;
  Index nadc = 4096;    // # adc bins
  Index iadc1 = 65;     // Lowest adc bin for report and histograms
  Index iadc2 = 4095;   // One past highest adc bin for report and histograms
  Index ndst = 0;            // # data samples (times)
  AdcCalibrationTree* pact = nullptr;  // Calib tree
  IndexVector calibEntries;  // Tree entry number for each of the ndst samples
  SampleIndexVector sampleCountTotals; // Total # counts for each of the ndst samples
  SampleIndexVector binCountTotals; // Total # counts for each of the ndst+1 bins
  TH1* phbad = nullptr;       // # times each bin is bad
  TH1* phgood = nullptr;      // # times each bin is bad
  TH1* phlostat = nullptr;    // # times each bin has low stats
  IndexVector  agoodBins; // ADC bins which are always good (0 < RMS < maxRMS)
  IndexVector   abadBins; // ADC bins which are always bad or low-stat
  IndexVector   varyBins; // ADC bins which are sometimes good and sometimes bad or low-stat
  IndexVector gbvaryBins; // ADC bins which are sometimes good and sometimes bad
  IndexVector   goodBins; // ADC bins which declared good (never bad, always or sometimes good)
  IndexVector    badBins; // ADC bins which declared good (never bad, always or sometimes good)

private:

  bool m_haveReport = false;
  mutable HistVector m_countHists; //!
  mutable HistVector m_rmsHists;   //!

};

#endif
