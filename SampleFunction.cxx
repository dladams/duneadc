// SampleFunction.cxx

#include "SampleFunction.h"
#include "TF1.h"

//**********************************************************************

TF1* SampleFunction::rootFun(Name name) const {
  return new TF1(name.c_str(), this, &SampleFunction::tf1, xmin(), xmax(), 0);
}

//**********************************************************************
