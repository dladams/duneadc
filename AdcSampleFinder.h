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

  // Ctor specifying the location of the data files.
  explicit AdcSampleFinder(Name topdir ="$HOME/data/dune/adc");

  // Return the location of the data files.
  Name topdir() const  { return m_topdir; }

  // Return the reader for a sample.
  // Returns null if the sample is not found or does not have the requested channel.
  // Channel is not checked if it is set to badChannel().
  AdcSampleReaderPtr find(Name asample, Index icha = badChannel(), Index maxsam =0) const;

  // Return a binary reader.
  AdcSampleReaderPtr findBinaryReader(Name asample, Index icha = badChannel(), Index maxsam =0) const;

  // Convert channel snumber to string.
  Name schan(Index icha) const;

private:


  std::string m_topdir;

};

#endif
