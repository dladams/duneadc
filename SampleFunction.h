// SampleFunction.h

#ifndef SampleFunction_H
#define SampleFunction_H

#include "AdcTypes.h"
#include "AdcExtremum.h"
#include <string>

class TF1;

class SampleFunction {

public:

  using Name = std::string;

  // Dtor.
  virtual ~SampleFunction() = default;

  // Return the value for a given bin.
  // If pdvds is not null, the slope dvalue/sample may be returned there.
  virtual double value(SampleIndex isam, double* pdvds =nullptr) const = 0;

  // Return the limits of validity.
  virtual SampleIndex xmin() const = 0;
  virtual SampleIndex xmax() const = 0;

  // For Root TF1.
  double tf1(double* x, double*) const { return value(x[0]); }

  // Return a TF1.
  TF1* rootFun(Name name ="SampleFunction") const;
  
};

#endif
