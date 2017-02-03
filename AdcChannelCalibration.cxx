// AdcChannelCalibration.cxx

#include "AdcChannelCalibration.h"

using Float = AdcChannelCalibration::Float;


//**********************************************************************

AdcChannelCalibration::AdcChannelCalibration()
: gain(0.0), offset(0.0) { }

//**********************************************************************

AdcChannelCalibration::AdcChannelCalibration(AdcChannelId aid)
: chip(aid.chip), chan(aid.chan), gain(0.0), offset(0.0) { }

//**********************************************************************

AdcChannelCalibration::
AdcChannelCalibration(AdcChannelId aid, Float again, Float aoffset,
                      const FloatVector& acalMeans,
                      const FloatVector& acalRmss,
                      const IndexVector& acalCounts)
: chip(aid.chip), chan(aid.chan), gain(again), offset(aoffset),
  calMeans(acalMeans), calRmss(acalRmss), calCounts(acalCounts) { }

//**********************************************************************

Float AdcChannelCalibration::calMean(Index code) const {
  if ( code >= calMeans.size() ) return -999.0;
  return calMeans[code];
}

//**********************************************************************

Float AdcChannelCalibration::calRms(Index code) const {
  if ( code >= calRmss.size() ) return -1.0;
  return calRmss[code];
}

//**********************************************************************

AdcChannelCalibration::Index AdcChannelCalibration::calCount(Index code) const {
  if ( code >= calCounts.size() ) return 0;
  return calCounts[code];
}

//**********************************************************************
