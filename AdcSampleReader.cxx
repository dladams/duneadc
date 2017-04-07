// AdcSampleReader.cxx

#include "AdcSampleReader.h"
#include "TH1.h"

//**********************************************************************

TH1* AdcSampleReader::histdata(unsigned int idat0, unsigned int ndatin, unsigned int show) {
  if ( idat0 >= nsample() ) return 0;
  if ( show < 1 ) return 0;
  double samper = 1.0;
  string xlab = "Tick";
  unsigned int ndat = ndatin;
  if ( ndat == 0 ) ndat = nsample() - idat0;
  if ( samplingFrequency() > 0.0 ) {
    xlab = "time [sec]";
    samper = 1.0/samplingFrequency();
  }
  string hname = "hwf";
  string htitl = "Waveform; " + xlab + "; ADC code";
  unsigned int npt = ndat/show;
  double t1 = samper*idat0;
  double t2 = samper*(t1 + ndat);
  TH1* ph = new TH1F(hname.c_str(), htitl.c_str(), npt, t1, t2);
  ph->SetStats(0);
  for ( unsigned int ipt=0; ipt<npt; ++ipt ) {
    unsigned int idat = show*ipt;
    Code adc = code(idat);
    ph->SetBinContent(ipt+1, adc);
  }
  return ph;
}

//**********************************************************************

double AdcSampleReader::vinLow(Index ivin) const {
  return vinmin() + dvin()*ivin;
}

//**********************************************************************

double AdcSampleReader::vinCenter(Index ivin) const {
  return vinmin() + dvin()*(ivin+0.5);
}

//**********************************************************************
