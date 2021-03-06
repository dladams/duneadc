// AdcChipMetric.cxx

#include "AdcChipMetric.h"
#include <iostream>
#include <iomanip>
#include "AdcPerformanceTree.h"
#include "AdcVoltagePerformance.h"
#include "TSystem.h"

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

AdcChipMetric::
AdcChipMetric(Name a_dataset, Name a_sampleName, Name a_perfFileName,
              Index a_firstChannel, Index a_nChannel)
: m_dataset(a_dataset),
  m_sampleName(a_sampleName),
  m_perfFileName(a_perfFileName),
  m_chip(badChip()),
  m_firstChannel(a_firstChannel),
  m_nChannel(a_nChannel),
  m_time(0),
  m_chanEff(m_nChannel, -1.0) { }

//**********************************************************************

AdcChipMetric::
AdcChipMetric(Name a_dataset, Name a_sampleName, Index a_firstChannel, Index a_nChannel)
: AdcChipMetric(a_dataset, a_sampleName, "", a_firstChannel, a_nChannel) { }

//**********************************************************************

int AdcChipMetric::evaluate() {
  const Name myname = "AdcChipMetric::evaluate: ";
  Name basefname = m_perfFileName;
  if ( basefname.size() == 0 ) basefname = "perf_" + dataset() + ".root";
  Name fname;
  if ( basefname.size() == 0 ) return -1;
  if ( basefname[0] == '.' || basefname[0] == '/' ) {
    fname = basefname;
  } else {
    const char* cdirpath = gSystem->Getenv("DUNEADC_FILE_PATH");
    string dirpath = (cdirpath == nullptr) ? "." : cdirpath;
    string::size_type ipos = 0;
    vector<string> dirs;
    while ( ipos < dirpath.size() ) {
      string::size_type jpos = dirpath.find(":", ipos);
      string dirname = dirpath.substr(ipos, jpos-ipos);
      string filepath = dirname + "/" + basefname;
      if ( ! gSystem->AccessPathName(filepath.c_str()) ) {
        fname = filepath;
        break;
      }
      dirs.push_back(dirname);
      if ( jpos == string::npos ) break;
      ipos = jpos + 1;
    }
    if ( fname == "" ) {
      cout << myname << "Unable to find file " << basefname << " in dirs:" << endl;
      for ( string dir : dirs ) {
        cout << myname << "  " << dir << endl;
      }
      return -2;
    }
  }
  AdcPerformanceTree apt(fname);
  if ( apt.status() != 0 ) {
    cout << myname << "Unable to find performance file " << fname << endl;
    return -3;
  }
  Index endChannel = firstChannel() + nChannel();
  Index nbad = 0;
  double chanEffSum = 0.0;
  double chanEffProd = 1.0;
  unsigned int nchan = 0;
  double chanEffLow = 1.0;
  double chanEffLow2 = 1.0;
  int n80 = 0;
  // Find the entries for the specified channel range.
  Index nent = apt.size();
  IndexVector ents(nChannel(), nent);
  if ( m_sampleName.size() ) {
    Index ient = 0;
    while ( ient < nent ) {
      const AdcVoltagePerformance* pavp = apt.findSample(ient, m_sampleName);
      if ( pavp != nullptr ) {
        Index icha = pavp->chan;
        if ( icha >= firstChannel() && icha < endChannel ) {
          Index kcha = icha - firstChannel();
          if ( ents[kcha] == nent ) {
            ents[kcha] = ient;
          } else {
            cout << myname << "Skipping duplicate channel " << icha << " for sample " << m_sampleName << endl;
          }
        }
        if ( chip() == badChip() ) {
          m_chip = pavp->chip;
        } else if ( pavp->chip != chip() ) {
          cout << myname << "WARNING: Chips have inconsistent numbering (" << chip() << ", "
               << pavp->chip << ") for sample " << sampleName() << endl;
        }
      }
      ++ient;
    }
  } else {
    for ( Index kcha=0; kcha<nChannel(); ++kcha ) {
      Index chan = firstChannel() + kcha;
      Index ient = 0;
      const AdcVoltagePerformance* pavp = apt.findNext(ient, chip(), chan);
      if ( pavp == nullptr ) {
        cout << myname << "Unable to find chip " << chip() << " channel " << chan << endl;
        setChannelEfficiency(chan, 0.0);
        ++nbad;
        chanEffProd = 0.0;
        chanEffLow = 0.0;
        chanEffLow2 = 0.0;
        break;
      } else {
        ents[kcha] = ient;
      }
    }
  }
  // Loop over channel entries.
  Index icha = firstChannel();
  for ( Index ient : ents ) {
    if ( ient == nent ) {
      cout << myname << "Unable to find channel " << icha << endl;
      setChannelEfficiency(icha, 0.0);
      ++nbad;
      chanEffProd = 0.0;
      chanEffLow = 0.0;
      chanEffLow2 = 0.0;
      continue;
    }
    const AdcVoltagePerformance* pavp = apt.find(ient);
    const AdcVoltagePerformance& avp = *pavp;
    if ( m_time == 0 ) m_time = avp.time;
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
    setChannelEfficiency(avp.chan, chanEff);
    chanEffSum += chanEff;
    chanEffProd *= chanEff;
    if ( chanEff < chanEffLow ) {
      chanEffLow2 = chanEffLow;
      chanEffLow = chanEff;
    } else if ( chanEff < chanEffLow2 ) {
      chanEffLow2 = chanEff;
    }
    if ( chanEff > 0.80 ) ++n80;
    //cout << chip() << " " << chan << ": " << chanEff << endl;
    ++icha;
    ++nchan;
  }
  double chipEffAvg = 0.0;
  if ( nchan > 0 && nchan == nChannel() ) chipEffAvg = chanEffSum/nchan;
  setChipMetric("EffAvg", chipEffAvg);
  setChipMetric("EffProd", chanEffProd);
  setChipMetric( "EffLow", chanEffLow);
  setChipMetric("EffLow2", chanEffLow2);
  setChipMetric(    "N80", n80);
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
  cout << "Metrics for sample " << sampleName() << " (";
  if ( dataset().size() ) cout << "dataset " << dataset() << " ";
  cout << "chip " << chip() << ")" << endl;
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


