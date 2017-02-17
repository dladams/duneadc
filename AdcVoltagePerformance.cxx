// AdcVoltagePerformance.cxx

#include "AdcVoltagePerformance.h"

using Count = AdcVoltagePerformance::Count;
using Float = AdcVoltagePerformance::Float;

//**********************************************************************

AdcVoltagePerformance::AdcVoltagePerformance()
: chip(0), chan(999), vrmsmax(0.0), nv(0), vmin(0.0), vmax(0.0) { }

//**********************************************************************

AdcVoltagePerformance::
AdcVoltagePerformance(Index achip, Index achan, Float avrmsmax,
                      Index anv, Float avmin, Float avmax)
: chip(achip), chan(achan), vrmsmax(avrmsmax),
  nv(anv), vmin(avmin), vmax(avmax),
  vinCounts(nv, 0),
  vinEffs(nv, 0.0) { }

//**********************************************************************

Float AdcVoltagePerformance::vlo(Index iv) const {
  Float dv = (vmax - vmin)/Float(nv);
  return vmin + Float(iv)*dv;
}

//**********************************************************************

Float AdcVoltagePerformance::vhi(Index iv) const {
  Float dv = (vmax - vmin)/Float(nv);
  return vmin + Float(iv+1)*dv;
}

//**********************************************************************

Count AdcVoltagePerformance::count(Index iv) const {
  if ( iv >= nv ) return 0;
  return vinCounts[iv];
}

//**********************************************************************

Float AdcVoltagePerformance::eff(Index iv) const {
  if ( iv >= nv ) return 0.0;
  return vinEffs[iv];
}

//**********************************************************************

Float AdcVoltagePerformance::deff(Index iv) const {
  Float val = eff(iv);
  Float nval = count(iv);
  double aval = 1.0 - val;
  double dval = 0.0;
  if ( val > 1.e-10 && aval > 1.e-10 ) dval = sqrt(val*(1.0-val)/nval);
  return dval;
}

//**********************************************************************
