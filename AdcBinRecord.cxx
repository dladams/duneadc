// AdcBinRecord.cxx

#include "AdcBinRecord.h"
#include <string>
#include <iostream>
#include <sstream>
#include "TH1.h"

using std::string;
using std::cout;
using std::endl;
using std::ostringstream;

//**********************************************************************
// Subclass methods.
//**********************************************************************

AdcBinRecord::Peak::Peak(const SampleIndexVector& sams) {
  if ( sams.size() == 0 ) return;
  size = sams.size();
  low = sams.front();
  high = sams.back();
  double sumOff = 0.0;
  double sumOff2 = 0.0;
  for ( SampleIndex isam : sams ) {
    double off = isam - low;
    sumOff += off;
    sumOff2 += off*off;
  }
  meanOffset = sumOff/size;
  rms = sqrt(sumOff2/size - meanOffset*meanOffset);
  truncMean = low + SampleIndex(meanOffset);
}

//**********************************************************************
// Class methods.
//**********************************************************************

AdcBinRecord::AdcBinRecord(AdcCode a_code, bool doHist)
: m_code(a_code), m_doHist(doHist), m_minGap(0) { }

//**********************************************************************

AdcBinRecord::AdcBinRecord(Name slab, bool doHist)
: m_code(0), m_slab(slab), m_doHist(doHist), m_minGap(0) { }

//**********************************************************************

AdcBinRecord::~AdcBinRecord() {
  for ( TH1*& ph : m_peakHists ) {
    delete ph;
    ph = nullptr;
  }
}

//**********************************************************************

int AdcBinRecord::addSample(SampleIndex isam) {
  string myname = "AdcBinRecord::addSample: ";
  if ( m_samples.size() && isam < m_samples.back() ) {
    cout << myname << "Sample is out of order. Skipping." << endl;
    return 1;
  }
  m_samples.push_back(isam);
  m_minGap = 0.0;
  m_peaks.clear();
  return 0;
}

//**********************************************************************

int AdcBinRecord::findPeaks(SampleIndex a_minGap) {
  const string myname = "AdcBinRecord::findPeaks: ";
  const int dbg = 0;
  m_minGap = a_minGap;
  m_peaks.clear();
  SampleIndexVector peakSams;
  Index ipeak = 0;
  // Loop one past the lsat sample so we can record the last peak.
  if ( dbg ) cout << myname << "Looping over << " << samples().size() << " samples." << endl;
  for ( SampleIndex ksam=0; ksam<=samples().size(); ++ksam ) {
    bool last = ksam == samples().size();
    SampleIndex isam = last ? 99999 : samples()[ksam];
    // If this is the end of a peak, record it.
    if ( peakSams.size() ) {
      bool bigGap = isam - peakSams.back() > minGap();
      if ( last || bigGap ) {
        m_peaks.emplace_back(peakSams);
        if ( m_doHist ) {
          SampleIndex isam1 = peakSams.front();
          SampleIndex isam2 = peakSams.back() + 1;
          ostringstream sshtitl;
          sshtitl << "Peak " << ipeak << " for ";
          if ( m_slab.size() ) sshtitl << m_slab;
          else sshtitl << "bin " << m_code;
          sshtitl << "; Tick - " << isam1;
          sshtitl << "; # samples";
          Name htitl = sshtitl.str();
          ostringstream sshname;
          sshname << "hpeak_";
          if ( m_slab.size() ) sshname << m_slab;
          else sshname << "bin " << m_code;
          string hname = sshname.str();
          for ( string::size_type ipos=0; ipos<hname.size(); ++ipos ) {
            char ch = hname[ipos];
            if ( ch == ' ' || ch == '-' ) hname[ipos] = '_';
          }
          int nx = peakSams.size();
          double x1 = 0.0;
          double x2 = isam2 - isam1;
          if ( nx > 100 ) {
            int oldnx = nx;
            while ( nx > 500 ) nx /= 10;
            if ( nx > 200 ) nx /= 5;
            if ( nx > 100 ) nx /= 2;
            int fac = oldnx/nx;
            x2 = fac*(int(x2/fac)+0.999);
          }
          TH1* ph = new TH1F(hname.c_str(), htitl.c_str(), nx, x1, x2);
          //ph->SetStats(0);
          ph->SetLineWidth(2);
          ph->SetDirectory(0);
          SampleIndex diff = isam-isam1;
          for ( SampleIndex isam : peakSams ) {
            double diff = isam - isam1;
            ph->Fill(diff);
          }
          m_peakHists.push_back(ph);
        }
        peakSams.clear();
        ++ipeak;
      }
    }
    peakSams.push_back(isam);
  }
  if ( dbg ) cout << myname << "Finished loop over samples." << endl;
  return m_peaks.size();
}

//**********************************************************************

const AdcBinRecord::Peak& AdcBinRecord::peak(Index ipeak) const {
  static Peak badPeak;
  if ( ipeak > npeak() ) return badPeak;
  return m_peaks[ipeak];
}

//**********************************************************************
