// AdcLinearChannelCalibration.cxx

#include "AdcLinearChannelCalibration.h"

//**********************************************************************

AdcLinearChannelCalibration::
AdcLinearChannelCalibration(double gain, double offset, Index nbin,
                            Index chan, Index chip, Index time)
: m_size(nbin),
  m_chip(chip), m_chan(chan), m_time(time),
  m_gain(gain), m_offset(offset) { }

//**********************************************************************

double AdcLinearChannelCalibration::calMean(AdcCode iadc) const {
  if ( iadc >= size() ) return 0.0;
  return linearGain()*iadc + linearOffset();
}

//**********************************************************************
