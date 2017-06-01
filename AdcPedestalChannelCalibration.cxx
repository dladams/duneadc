// AdcPedestalChannelCalibration.cxx

#include "AdcPedestalChannelCalibration.h"

//**********************************************************************

AdcPedestalChannelCalibration::
AdcPedestalChannelCalibration(const AdcChannelCalibration& calraw, double ped)
: m_calraw(calraw), m_ped(ped) { }

//**********************************************************************

AdcPedestalChannelCalibration::
AdcPedestalChannelCalibration(const AdcChannelCalibration& calraw,
                              Index iadc1, Index a_iadc2,
                              double vped,
                              double rmsmax)
: m_calraw(calraw), m_ped(0.0) {
  Index nbin = 0;
  double vsum = 0.0;
  Index iadc2 = a_iadc2 > iadc1 ? a_iadc2 : iadc1 + 1;
  for ( Index iadc=iadc1; iadc<iadc2; ++iadc ) {
    if ( iadc >= size() ) break;
    if ( calRms(iadc) < rmsmax ) {
      vsum += calMean(iadc);
      ++nbin;
    }
  }
  if ( nbin > 0 ) {
    m_ped = vsum/nbin;
  }
}

//**********************************************************************

double AdcPedestalChannelCalibration::calMean(AdcCode iadc) const {
  if ( iadc >= size() ) return 0.0;
  return rawCalibration().calMean(iadc) - pedestal();
}

//**********************************************************************

