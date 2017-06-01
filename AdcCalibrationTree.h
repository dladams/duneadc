// AdcCalibrationTree.h

// David Adams
// January 2017
//
// Class to hold ADC calibration data in a Root TTree.

#ifndef AdcCalibrationTree_H
#define AdcCalibrationTree_H

#include <string>
#include "AdcTreeChannelCalibration.h"

class TTree;
class TFile;

class AdcCalibrationTree {

public:

  using Name = std::string;
  using Index = unsigned int;

  // Ctor from a tree.
  //   fname = file name
  //   tname = tree name
  //   opt = Option for opening file: READ, NEW, RECREATE, UPDATE
  AdcCalibrationTree(Name fname, Name tname ="adccalib", Name opt ="READ");

  // Dtor. Closes file.
  ~AdcCalibrationTree();

  // Add a calibration.
  int insert(const AdcTreeChannelCalibration& cal);

  // Add the current buffered calibration.
  int insert();

  // Close the file.
  int close();

  // Return the number of entries.
  Index size() const;

  // Find a calibration by entry number.
  const AdcTreeChannelCalibration* find(Index ient) const;

  // Find a calibration by ID. Search starts at ient and continues to the
  // end of the tree. The index ient is entry number of the returned calibration.
  const AdcTreeChannelCalibration* find(AdcChannelId id, Index& ient) const;
  const AdcTreeChannelCalibration* find(Index chip, Index chan, Index& ient) const;

  // Accessors.
  int status() const { return m_status; }
  TFile* file() const { return m_pfile; }
  TTree* tree() const { return m_ptree; }
  AdcTreeChannelCalibration& buffer() { return *m_pcal; }

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
  AdcTreeChannelCalibration* m_pcal;
  bool m_modified;

};

#endif
