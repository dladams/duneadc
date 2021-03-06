// AdcSampleFinder.h

// David Adams
// May 2017

// Class to find samples identified by sample name and
// channel number.

#ifndef AdcSampleFinder_H
#define AdcSampleFinder_H

#include <memory>
#include "AdcSampleReader.h"

class AdcSampleFinder {

public:

  using Name = std::string;
  using AdcSampleReaderPtr = std::unique_ptr<AdcSampleReader>;

  // Directory where ADC data files are found.
  // Non-blank value is used to set the value on the first call.
  // If no value is provided on the first call, the default
  // $HOME/data/dune/adc is used.
  // After the first call, the argument is ignored.
  static Name defaultTopdir(Name setValue ="");

  // Return if a DUNE17 sample is bad.
  // if chan = badChannel(), then returns if any of the channels are bad.
  static bool isBadDune17(Index chip, Index chan =badChannel(), AdcTime time =0);

  // Ctor specifying the location of the data files.
  // If blank, defaultTopdir() is used.
  explicit AdcSampleFinder(Name a_topdir ="");

  // Return the location of the data files.
  Name topdir() const  { return m_topdir; }

  // Return the reader for a sample.
  // Returns null if the sample is not found or does not have the requested channel.
  // Channel is not checked if it is set to badChannel().
  AdcSampleReaderPtr find(Name asample, Index icha = badChannel(), SampleIndex maxsam =0, bool calculateVin =true) const;

private:

  // Return a binary reader.
  AdcSampleReaderPtr findBinaryReader(Name asample, Index icha = badChannel(), SampleIndex maxsam =0) const;

  // Return a FEMB reader.
  AdcSampleReaderPtr findFembReader(Name asample, Index icha, SampleIndex maxsam =0, bool calculateVin =true) const;

  // Return a quad reader.
  AdcSampleReaderPtr findQuadReader(Name asample, Index icha, SampleIndex maxsam =0) const;

  // Convert channel snumber to string.
  // Returns {0, 1, ... F} if useup is true, else {0, 1, ..., f}
  Name schan(Index icha, bool useup =true) const;

private:


  std::string m_topdir;

};

#endif
