// Sawtooth.h

#ifndef Sawtooth_H
#define Sawtooth_H

#include "SampleFunction.h"

class Sawtooth : public SampleFunction {

public:

  // Ctor from extrema and limits.
  Sawtooth(double vmin, double vmax, const AdcExtrema exts);

  // Return the value for a given bin.
  double value(SampleIndex isam, double* pdvds) const override;

  // Return range of validity.
  SampleIndex xmin() const override { return m_isamLow; }
  SampleIndex xmax() const override { return m_isamHigh; }

private:

  double m_vmin;
  double m_vmax;
  SampleIndex m_isamMin;
  SampleIndex m_wsam;
  SampleIndex m_isamLow;
  SampleIndex m_isamHigh;

};

#endif
