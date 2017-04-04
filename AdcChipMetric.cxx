// AdcChipMetric.cxx

#include "AdcChipMetric.h"
#include <iostream>
#include <iomanip>
#include "AdcPerformanceTree.h"
#include "AdcVoltagePerformance.h"

using std::cout;
using std::endl;
using std::setw;
using std::fixed;

using Name = AdcChipMetric::Name;
using Index = AdcChipMetric::Index;

//**********************************************************************

AdcChipMetric::
AdcChipMetric(Name a_dataset, Index a_chip, Index a_firstChannel, Index a_nChannel)
: m_dataset(a_dataset),
  m_chip(a_chip),
  m_firstChannel(a_firstChannel),
  m_nChannel(a_nChannel),
  m_chanEff(m_nChannel, -1.0) { }
  
//**********************************************************************

int AdcChipMetric::evaluate() {
  const Name myname = "AdcChipMetric::evaluate: ";
  Name fname = "perf_" + dataset() + ".root";
  AdcPerformanceTree apt(fname);
  if ( apt.status() != 0 ) {
    cout << myname << "Unable to find performance file " << fname << endl;
    return -1;
  }
  Index endChannel = firstChannel() + nChannel();
  Index nbad = 0;
  double chanEffSum = 0.0;
  double chanEffProd = 1.0;
  unsigned int nchan = 0;
  double chanEffLow = 1.0;
  for ( Index chan=firstChannel(); chan<endChannel; ++chan ) {
    const AdcVoltagePerformance* pavp = apt.find(chip(), chan);
    if ( pavp == nullptr ) {
      cout << myname << "Unable to find chip " << chip() << " channel " << chan << endl;
      setChannelEfficiency(chan, 0.0);
      ++nbad;
      chanEffProd = 0.0;
      chanEffLow = 0.0;
    } else {
      const AdcVoltagePerformance& avp = *pavp;
      double vin = avp.vmin;
      double vmin = 200.0;
      double vmax = 1200.0;
      double close = 1.0;
      double v1 = 0.0;
      double v2 = avp.vmin;
      double dv = (avp.vmax - avp.vmin)/avp.nv;
      double effsum = 0.0;
      double neff = 0;
      for ( Index iv=0; iv<avp.nv; ++iv ) {
        v1 = v2;
        v2 += dv;
        if ( v1 + close < vmin ) continue;
        if ( v2 - close > vmax ) break;
        double eff = avp.eff(iv);
        // cout << chan << " " << iv << ": " << eff << endl;
        ++neff;
        effsum += eff;
      }
      double chanEff = effsum/neff;
      setChannelEfficiency(chan, chanEff);
      chanEffSum += chanEff;
      chanEffProd *= chanEff;
      if ( chanEff < chanEffLow ) chanEffLow = chanEff;
      //cout << chip() << " " << chan << ": " << chanEff << endl;
      ++nchan;
    }
  }
  double chipEffAvg = 0.0;
  if ( nchan > 0 && nchan == nChannel() ) chipEffAvg = chanEffSum/nchan;
  setChipMetric("EffAvg", chipEffAvg);
  setChipMetric("EffProd", chanEffProd);
  setChipMetric("EffLow", chanEffLow);
  return nbad;
}
    
//**********************************************************************

double AdcChipMetric::channelEfficiency(Index chan) const {
  if ( chan < firstChannel() ) return -1.0;
  Index ichan = chan - firstChannel();
  if ( ichan >= nChannel() ) return -1.0;
  return m_chanEff[ichan];
}

//**********************************************************************

double AdcChipMetric::chipMetric(Name name) const {
  MetricMap::const_iterator ient = chipMetrics().find(name);
  if ( ient == chipMetrics().end() ) return -1.0;
  return ient->second;
}

//**********************************************************************

void AdcChipMetric::print() const {
  cout << "Metrics for dataset " << dataset() << " chip " << chip() << endl;
  Index endChannel = firstChannel() + nChannel();
  Index saveprec = cout.precision();
  cout.precision(4);
  for ( Index chan=firstChannel(); chan<endChannel; ++chan ) {
    cout << setw(4) << chan << ": " << setw(8) << fixed << channelEfficiency(chan) << endl;
  }
  for ( const MetricMap::value_type& ent : chipMetrics() ) {
    cout << setw(10) << ent.first << ":" << setw(8) << fixed << ent.second << endl;
  }
  cout.precision(saveprec);
}

//**********************************************************************

int AdcChipMetric::setChannelEfficiency(Index chan, double val) {
  if ( chan < firstChannel() ) return 1;
  Index ichan = chan - firstChannel();
  if ( ichan >= nChannel() ) return 2;
  m_chanEff[ichan] = val;
  return 0;
}

//**********************************************************************

int AdcChipMetric::setChipMetric(Name name, double val) {
  m_chipMetric[name] = val;
  return 0;
}

//**********************************************************************

