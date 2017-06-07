// AdcPedestalChannelCalibration.cxx

#include "AdcPedestalChannelCalibration.h"

//**********************************************************************

double AdcPedestalChannelCalibration::
evaluatePedestal(const AdcChannelCalibration& cal,
                 Index iadc1, Index a_iadc2, double rmsmax) {
  Index nbin = 0;
  double vsum = 0.0;
  Index iadc2 = a_iadc2 > iadc1 ? a_iadc2 : iadc1 + 1;
  for ( Index iadc=iadc1; iadc<iadc2; ++iadc ) {
    if ( iadc >= cal.size() ) break;
    double rms = cal.calRms(iadc);
    if ( rms > 0 && rms < rmsmax ) {
      vsum += cal.calMean(iadc);
      ++nbin;
    }
  }
  return nbin > 0 ? vsum/nbin : 0.0;
}

//**********************************************************************

AdcPedestalChannelCalibration::
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
  double pedin = evaluatePedestal(calraw, iadc1, a_iadc2, rmsmax);
  m_ped = pedin - vped;
}

//**********************************************************************

AdcPedestalChannelCalibration::
AdcPedestalChannelCalibration(const AdcChannelCalibration& calraw,
                              Index iadc1, Index a_iadc2,
                              const AdcChannelCalibration& cal0,
                              double rmsmax)
: m_calraw(calraw), m_ped(0.0) {
  double ped0 = evaluatePedestal(cal0, iadc1, a_iadc2, rmsmax);
  double ped = evaluatePedestal(calraw, iadc1, a_iadc2, rmsmax);
  m_ped = ped - ped0;
}

//**********************************************************************

double AdcPedestalChannelCalibration::calMean(AdcCode iadc) const {
  if ( iadc >= size() ) return 0.0;
  return rawCalibration().calMean(iadc) - pedestal();
}

//**********************************************************************

