// TH1Props.h

#ifndef TH1Props_H
#define TH1Props_H

class TH1;

// Calculate properties of a TH1 histogram.
//
// Definitions:
//   deviation = the distance between furthest bin edge and the histogram mean.
//        pull = deviation/RMS
class TH1Props {

public:

  // Ctor from histogram.
  TH1Props(TH1* ph);

  // Find the bin whose center is below or at the specified value.
  unsigned int findBinCenterBelow(double x) const;

  // Find the bin whose center is above the specified value.
  unsigned int findBinCenterAbove(double x) const;

  // Return the minimum pull, i.e. the pull for the first bin with > 0 entries.
  double pullMin() const;

  // Return the minimum pull, i.e. the pull for the first bin with > 0 entries.
  double pullMax() const;

  // Return the expanded RMS.
  // If the maximimum abs(pull) > apullmax, then the RMS is expanded so that
  // abs(pull) is pullmax.
  double expandedRMS(double apullmax) const;

  // Return the fraction of entries with pull < apullthresh.
  double tailFracLo(double apullthresh) const;

  // Return the fraction of entries with pull > apullthresh.
  double tailFracHi(double apullthresh) const;

  // Return the fraction of entries with |pull| > apullthresh.
  double tailFrac(double apullthresh) const;

  // Return the fraction of entries the specified distance below the mean.
  double fracBelowMean(double dist) const;

  // Return the fraction of entries the specified distance above the mean.
  double fracAboveMean(double dist) const;

  // Return the fraction of entries the specified distance above or below the mean.
  double fracOutsideMean(double dist) const;

  TH1* ph;

};

#endif
