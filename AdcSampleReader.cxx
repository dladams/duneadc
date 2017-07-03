// AdcSampleReader.cxx

#include "AdcSampleReader.h"
#include <sstream>
#include "TH1.h"
#include "TF1.h"

using std::ostringstream;

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
histdata(SampleIndex idat0, SampleIndex ndatin,
         int sshow, bool usetime) const {
  if ( idat0 >= nsample() ) return 0;
  if ( sshow == 0 ) return 0;
  unsigned int show = std::abs(sshow);
  bool binned = sshow < 0;
  double samper = 1.0;
  string xlab = "Tick";
  unsigned int ndat = ndatin;
  if ( ndat == 0 ) ndat = nsample() - idat0;
  if ( usetime && samplingFrequency() > 0.0 ) {
    xlab = "time [sec]";
    samper = 1.0/samplingFrequency();
  }
  string hname = "hwf";
  ostringstream sstitl;
  sstitl << sample();
  if ( channel() != badChannel() ) sstitl << " channel " << channel();
  if ( binned ) sstitl << " binned";
  sstitl << " waveform";
  sstitl << "; " + xlab + "; ADC code";
  string htitl = sstitl.str();
  unsigned int npt = ndat/show;
  double t1 = samper*idat0;
  double t2 = samper*(t1 + ndat);
  TH1* ph = new TH1F(hname.c_str(), htitl.c_str(), npt, t1, t2);
  ph->SetStats(0);
  ph->SetDirectory(nullptr);
  for ( unsigned int ipt=0; ipt<npt; ++ipt ) {
    unsigned int idat = show*ipt;
    if ( binned ) {
      double adcMin = code(idat);
      double adcMax = adcMin;
      for ( SampleIndex jdat=idat+1; jdat<idat+show; ++jdat ) {
        double adcj = code(jdat);
        if ( adcj < adcMin ) adcMin = adcj;
        if ( adcj > adcMax ) adcMax = adcj;
      }
      double adcMean = 0.5*(adcMin + adcMax);
      double adcErr = 0.5*(adcMax - adcMin);
      ph->SetBinContent(ipt+1, adcMean);
      ph->SetBinError(ipt+1, adcErr);
    } else {
      Code adc = code(idat);
      ph->SetBinContent(ipt+1, adc);
    }
  }
  return ph;
}

//**********************************************************************

TH1* AdcSampleReader::
histvin(SampleIndex idat0, SampleIndex ndatin,
        unsigned int show, bool usetime) const {
  if ( idat0 >= nsample() ) return nullptr;
  if ( show < 1 ) return nullptr;
  double samper = 1.0;
  string xlab = "Tick";
  unsigned int ndat = ndatin;
  if ( ndat == 0 ) ndat = nsample() - idat0;
  if ( usetime && samplingFrequency() > 0.0 ) {
    xlab = "time [sec]";
    samper = 1.0/samplingFrequency();
  }
  string hname = "hvin";
  ostringstream sstitl;
  sstitl << sample();
  if ( channel() != badChannel() ) sstitl << " channel " << channel();
  sstitl << " V_{in}";
  sstitl << "; " + xlab + "; Voltage [mV]";
  string htitl = sstitl.str();
  unsigned int npt = ndat/show;
  double t1 = samper*idat0;
  double t2 = samper*(t1 + ndat);
  TH1* ph = new TH1F(hname.c_str(), htitl.c_str(), npt, t1, t2);
  ph->SetStats(0);
  ph->SetDirectory(nullptr);
  for ( unsigned int ipt=0; ipt<npt; ++ipt ) {
    unsigned int idat = show*ipt;
    ph->SetBinContent(ipt+1, vin(idat));
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

int AdcSampleReader::
buildTableFromWaveform(Index a_nvin, double a_dvin, double a_vinmin,
                       bool doAllTable, bool doSlopeTables) {
  const Name myname = "AdcSampleReader::buildTableFromWaveform: ";
  const SampleIndex buildLogPeriod = 0;
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
  m_tableu.clear();
  m_tabled.clear();
  m_table.resize(nadc());
  for ( CountVector& vec : m_table ) vec.resize(nvin());
  if ( doSlopeTables ) {
    m_tableu.resize(nadc());
    for ( CountVector& vec : m_tableu ) vec.resize(nvin());
    m_tabled.resize(nadc());
    for ( CountVector& vec : m_tabled ) vec.resize(nvin());
  }
  Index nunderflow = 0;
  Index noverflow = 0;
  for ( SampleIndex isam=0; isam<nsample(); ++isam ) {
    Code iadc = code(isam);
    double dvds = 0.0;
    double* pdvds = doSlopeTables ? &dvds : nullptr;
    double xvin = vin(isam, pdvds);
    if ( buildLogPeriod && isam && buildLogPeriod*(isam/buildLogPeriod) == isam ) {
      cout << myname << "Table build is at sample " << isam << " of " << nsample()
           << " (" << int((100.0*isam)/nsample()) << "%)" << endl;
    }
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
      m_tableu.clear();
      m_tabled.clear();
      return 11;
    }
    if ( iadc >= m_table.size() ) {
      cout << myname << "ERROR: Invalid ADC code: " << iadc << endl;
      m_table.clear();
      m_tableu.clear();
      m_tabled.clear();
      return 12;
    }
    if ( ivin >= m_table[iadc].size() ) {
      cout << myname << "ERROR: Table row " << iadc << " is too small for index " << ivin << endl;
      cout << myname << "ERROR:   Row size is " << m_table[iadc].size() << endl;
      cout << myname << "ERROR:   isam = " << isam << endl;
      cout << myname << "ERROR:   Vin = " << xvin << endl;
      m_table.clear();
      m_tableu.clear();
      m_tabled.clear();
      return 13;
    }
    ++m_table[iadc][ivin];
    if ( dvds > 0.0 ) ++m_tableu[iadc][ivin];
    if ( dvds < 0.0 ) ++m_tabled[iadc][ivin];
  }
  m_haveVinSlopeTables = doSlopeTables;
  if ( nunderflow ) cout << myname << "Table underflow count: " << nunderflow << endl;
  if ( noverflow ) cout << myname << "Table overflow count: " << noverflow << endl;
  return 0;
}

//**********************************************************************
