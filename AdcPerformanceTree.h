// AdcPerformanceTree.h

// David Adams
// January 2017
//
// Class to hold ADC performance data in a Root TTree.

#ifndef AdcPerformanceTree_H
#define AdcPerformanceTree_H

#include <string>
#include "AdcVoltagePerformance.h"

class TTree;
class TFile;

class AdcPerformanceTree {

public:

  using Name = std::string;
  using Index = unsigned int;

  // Ctor from a tree.
  //   fname = file name
  //   tname = tree name
  //   opt = Option for opening file: READ, NEW, RECREATE, UPDATE
  AdcPerformanceTree(Name fname, Name tname ="adcperf", Name opt ="READ");

  // Dtor. Closes file.
  ~AdcPerformanceTree();

  // Add a performance.
  int insert(const AdcVoltagePerformance& perf);

  // Add the current buffered calibration.
  int insert();

  // Close the file.
  int close();

  // Return the number of entries.
  Index size() const;

  // Find a performance by entry number.
  const AdcVoltagePerformance* find(Index ient) const;

  // Find a performance by ID and voltage threshold.
  // The latter is not used if it is zero.
  const AdcVoltagePerformance* find(AdcChannelId id, float vrmsmax =0) const;
  const AdcVoltagePerformance* find(Index chip, Index chan, float vrmsmax =0) const;

  // Find the next entry starting at ient.
  // ient is updated to pint at the entry.
  const AdcVoltagePerformance* findNext(Index& ient, Index chip, Index chan) const;

  // Accessors.
  int status() const { return m_status; }
  TFile* file() const { return m_pfile; }
  TTree* tree() const { return m_ptree; }
  AdcVoltagePerformance& buffer() { return *m_pperf; }

private:

  // Create the tree. Returns 0 for success.
  // Fails if tree or any object with name m_tname already exists.
  // CXalled by the ctor.
  int maketree();

  int m_status;
  Name m_fname;
  Name m_tname;
  TFile* m_pfile;
  TTree* m_ptree;

  // Data buffer
  AdcVoltagePerformance* m_pperf;
  bool m_writetree;

};

#endif
