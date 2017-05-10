// AdcSampleReader.cxx

#include "AdcSampleReader.h"
#include "TH1.h"
#include "TF1.h"

//**********************************************************************

TF1* AdcSampleReader::vinTF1(Name name) const {
  return new TF1(name.c_str(), this, &AdcSampleReader::vinForTF1, 0, nsample(), 0);
}
  
//**********************************************************************

double AdcSampleReader::vinForTF1(double* x, double*) const {
  return vin(x[0]);
}

//**********************************************************************

TH1* AdcSampleReader::
histdata(unsigned int idat0, unsigned int ndatin,
         unsigned int show, bool usetime) {
  if ( idat0 >= nsample() ) return 0;
  if ( show < 1 ) return 0;
  double samper = 1.0;
  string xlab = "Tick";
  unsigned int ndat = ndatin;
  if ( ndat == 0 ) ndat = nsample() - idat0;
  if ( usetime && samplingFrequency() > 0.0 ) {
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

TH1* AdcSampleReader::
histvin(unsigned int idat0, unsigned int ndatin,
        unsigned int show, bool usetime) {
  if ( idat0 >= nsample() ) return 0;
  if ( show < 1 ) return 0;
  double samper = 1.0;
  string xlab = "Tick";
  unsigned int ndat = ndatin;
  if ( ndat == 0 ) ndat = nsample() - idat0;
  if ( usetime && samplingFrequency() > 0.0 ) {
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
    double yvin = vin(idat);
    ph->SetBinContent(ipt+1, yvin);
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

int AdcSampleReader::buildTableFromWaveform(Index a_nvin, double a_dvin, double a_vinmin) {
  const Name myname = "AdcSampleReader::buildTableFromWaveform: ";
  if ( nsample() == 0 ) {
    cout << myname << "ERROR: Waveform is empty." << endl;
    return 1;
  }
  if ( a_nvin == 0 ) {
    cout << myname << "ERROR: # voltage bins is zero." << endl;
    return 2;
  }
  if ( a_dvin == 0.0 ) {
    cout << myname << "ERROR: Voltage bin width is too small." << endl;
    return 3;
  }
  m_nvin = a_nvin;
  m_dvin = a_dvin;
  m_vinmin = a_vinmin;
  m_table.clear();
  m_table.resize(nadc());
  for ( CountVector& vec : m_table ) vec.resize(nvin());
  Index nunderflow = 0;
  Index noverflow = 0;
  for ( SampleIndex isam=0; isam<nsample(); ++isam ) {
    Code iadc = code(isam);
    double xvin = vin(isam);
    if ( xvin < vinmin() ) {
      ++nunderflow;
      continue;
    }
    if ( xvin >= vinmax() ) {
      ++noverflow;
      continue;
    }
    Index ivin = (xvin-vinmin())/dvin();
    if ( ivin >= nvin() ) {
      cout << myname << "ERROR: Voltage index is out of range: " << ivin << " >= " << nvin() << endl;
      m_table.clear();
      return 11;
    }
    if ( iadc >= m_table.size() ) {
      cout << myname << "ERROR: Invalid ADC code: " << iadc << endl;
      m_table.clear();
      return 12;
    }
    if ( ivin >= m_table[iadc].size() ) {
      cout << myname << "ERROR: Table row " << iadc << " is too small for index " << ivin << endl;
      cout << myname << "ERROR:   Row size is " << m_table[iadc].size() << endl;
      cout << myname << "ERROR:   isam = " << isam << endl;
      cout << myname << "ERROR:   Vin = " << xvin << endl;
      m_table.clear();
      return 13;
    }
    ++m_table[iadc][ivin];
  }
  if ( nunderflow ) cout << myname << "Table underflow count: " << nunderflow << endl;
  if ( noverflow ) cout << myname << "Table overflow count: " << noverflow << endl;
  return 0;
}

//**********************************************************************
