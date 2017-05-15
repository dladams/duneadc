// AdcVoltagePerformance.cxx

#include "AdcVoltagePerformance.h"

using Count = AdcVoltagePerformance::Count;
using Float = AdcVoltagePerformance::Float;

//**********************************************************************

AdcVoltagePerformance::AdcVoltagePerformance()
: chip(0), chan(999), time(0), vuncmax(0.0), nv(0), vmin(0.0), vmax(0.0) { }

//**********************************************************************

AdcVoltagePerformance::
AdcVoltagePerformance(Index a_chip, Index a_chan, AdcTime a_time,
                      Float a_vuncmax,
                      Index a_nv, Float a_vmin, Float a_vmax)
: chip(a_chip), chan(a_chan), time(a_time),
  vuncmax(a_vuncmax),
  nv(a_nv), vmin(a_vmin), vmax(a_vmax),
  vinCounts(nv, 0),
  vinEffs(nv, 0.0),
  vinResMeans(nv, 0.0),
  vinResRmss(nv, 0.0),
  vinResRmslos(nv, 0.0),
  vinResRmshis(nv, 0.0) { }

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

Float AdcVoltagePerformance::resMean(Index iv) const {
  if ( iv >= nv ) return 0.0;
  return vinResMeans[iv];
}

//**********************************************************************

Float AdcVoltagePerformance::resRms(Index iv) const {
  if ( iv >= nv ) return 0.0;
  return vinResRmss[iv];
}

//**********************************************************************

Float AdcVoltagePerformance::resRmslo(Index iv) const {
  if ( iv >= nv ) return 0.0;
  return vinResRmslos[iv];
}

//**********************************************************************

Float AdcVoltagePerformance::resRmshi(Index iv) const {
  if ( iv >= nv ) return 0.0;
  return vinResRmshis[iv];
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
