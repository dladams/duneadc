// AdcDatasetAnalyzer.h

// David Adams
// March 2017
//
// Class to compare results for multiple dataset calibrations.

#ifndef AdcDatasetAnalyzer_H
#define AdcDatasetAnalyzer_H

#include <string>
#include <vector>

class TH2;

class AdcDatasetAnalyzer {

public:

  using Index = unsigned int;
  using IndexVector = std::vector<Index>;
  using Name = std::string;
  using NameVector = std::vector<Name>;
  using TH2Vector = std::vector<TH2*>;

  // Ctor.
  AdcDatasetAnalyzer(Index achip, Index ibin1 =1, Index ibin2 =4095);
  AdcDatasetAnalyzer(const NameVector& adsts, Index achip, Index ibin1 =1, Index ibin2 =4095);

  // Draw a histogram for channel chan.
  //   name = res, resj or cnt
  void draw(Name name, Index chan);

  // Draw histograms for channels (chan1, chan2].
  //   name = res, resj or cnt
  void draw(Name name, Index chan1, Index chan2);

  // Display the report for channel chan.
  void report(Index chan) const;

  // Display the report for all channels.
  void report(Name prefix ="");

  // Fill the histograms for channel chan.
  // No action if already filled.
  int fill(Index chan);

  // Data.
  NameVector dsts;
  Index chip;
  Index ibin1;
  Index ibin2;   // Range for jumping plot is (ibin1, ibin2]
  // Histograms. One entry for each channel.
  TH2Vector hres;    // Resolution vs. dataset vs. ADC bin
  TH2Vector hresj;   // Resolution vs. dataset vs. ADC bin for jumping bins
  TH2Vector hcnt;    // Sample count vs. dataset vs. ADC bin
  // Stats. One entry for each channel.
  IndexVector    chanNLowStat;   // # bins with low stats
  IndexVector  chanNAlwaysBad;   // # bins always bad
  IndexVector chanNAlwaysGood;   // # bins always good
  IndexVector       chanNJump;   // # bins that jump between good and bad
  IndexVector       chanNElse;   // # bins that are none of the above
  // Debug flag.
  bool dbg = 0;

};

#endif
