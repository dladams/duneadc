// AdcBinarySampleReader.cxx

#include "AdcBinarySampleReader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "TTree.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TLine.h"

using std::ostringstream;

using Name = AdcBinarySampleReader::Name;
using AdcCode = AdcBinarySampleReader::AdcCode;

//**********************************************************************

AdcBinarySampleReader::AdcBinarySampleReader(Name fname, SampleIndex afence)
: m_underflowCode(0),
  m_overflowCode(4095),
  m_chanMask(0xfff),
  m_chanShift(12),
  m_ownStream(true),
  m_pin(nullptr),
  m_fname(fname),
  m_fence(afence),
  m_nsample(0),
  m_haveReadFile(false),
  m_channel(0),
  m_ptree(nullptr),
  m_avgBins("peak averages", true)
  {
  std::ifstream* pin = new std::ifstream;
  m_pin = pin;
  pin->open(fname.c_str(), ios::binary);
}

//**********************************************************************

AdcBinarySampleReader::~AdcBinarySampleReader() {
  if ( m_ownStream ) delete m_pin;
  m_pin = nullptr;
}

//**********************************************************************

int AdcBinarySampleReader::read() {
  Name myname = "AdcBinarySampleReader::read: ";
  bool doBins = m_doBins && m_abrs.size() == 0;
  bool doTree = m_doTree && m_ptree == nullptr;
  bool doData = m_doData && m_data.size() == 0;
  if ( m_pin == nullptr ) {
    cout << myname << "No input stream." << endl;
    return 1;
  }
  istream& fin = *m_pin;
  fin.seekg(0, fin.end);
  m_nsample = fin.tellg()/2;
  fin.seekg(0, fin.beg);
  if ( nsample() == 0 ) {
    cout << myname << "Input stream is empty." << endl;
    return 2;
  }
  cout << myname << "  # samples: " << setw(10) << nsample() << endl;
  SampleIndex maxCount = m_nsample;
  if ( !doBins && !doTree && !doData ) maxCount = m_nDump;
  cout << myname << "  # to read: " << setw(10) << maxCount << endl;
  if ( maxCount == 0 ) return 0;
  Name fname = "AdcBinaryTree.root";
  TDirectory* olddir = gDirectory;
  TFile* ptreeFile = 0;
  AdcCode code;
  Index chan;
  if ( doTree ) {
    ptreeFile = TFile::Open(fname.c_str(), "RECREATE");
    if ( ptreeFile == nullptr || !ptreeFile->IsOpen() ) {
      cout << myname << "Unable to open tree file " << fname << endl;
      return 3;
    }
    m_ptree = new TTree("adcdata", "ADC data tree");
    m_ptree->Branch("code", &code, "code/s");
  }
  if ( doBins && m_abrs.size() == 0) {
    m_abrs.reserve(4096);
    for ( AdcCode code=0; code<4096; ++code ) m_abrs.emplace_back(code, false);
  }
  if ( doData ) {
    m_data.clear();
    m_data.resize(maxCount);
  }
  // Loop over block reads.
  SampleIndex l1 = 1;
  const int nsambuf = l1 << 19;
  AdcCode buff[nsambuf];
  cout << myname << "Buffer size: " << setw(10) << nsambuf << endl;
  SampleIndex ksam = 0;     // sample position in the stream
  SampleIndex isam = 0;     // sample position in the buffer.
  SampleIndex count = 0;    // Counter to check each sample is read.
  unsigned int ndumped = 0;
  while ( count < maxCount ) {
    fin.seekg(2*ksam);
    SampleIndex ksamNext = ksam + nsambuf;
    if ( ksamNext > nsample() ) ksamNext = nsample();
    SampleIndex nsamRead = ksamNext - ksam;
    if ( nsamRead == 0 ) break;
    fin.read((char*)buff, 2*nsamRead);
    cout << "Read block at sample " << setw(8) << ksam << ": " << setw(6) << (buff[0]&chanMask())
         << " (size = " << nsamRead << ")" << endl;
    SampleIndex isamMax = isam + nsamRead;
    for ( SampleIndex isam=0; isam<isamMax; (++isam, ++ksam) ) {
      if ( count >= maxCount ) break;
      AdcCode chancode = buff[isam];
      code = chancode&chanMask();
      Index chan = chancode>>chanShift();
      if ( ksam == 0 ) m_channel = chan;
      if ( m_nDump > ndumped ) {
        cout << setw(10) << count << ": channel= " << setw(2) << chan << ", code="
             << setw(4) << code << endl;
         ++ndumped;
      } else if ( ksam == 0 ) {
        cout << "Channel number is " << channel() << endl;
      }
      if ( doTree ) m_ptree->Fill();
      if ( doBins ) {
        if ( code >= m_abrs.size() ) abort();
        m_abrs[code].addSample(ksam);
      }
      if ( doData ) m_data[ksam] = code;
      ++count;
    }
  }
  if ( doBins || doTree ) {
    if ( count != nsample() ) {
      cout << "WARNING: Error counting samples: " << count << " != " << nsample() << endl;
    }
  }
  if ( ksam != count ) {
    cout << "ERROR: Sample index and count are inconsistent: " << ksam << " != " << count << endl;
  }
  if ( doBins ) {
    multiset<SampleIndex> peakAvgs;
    for ( AdcBinRecorder& abr : m_abrs ) {
      abr.findPeaks(fence());
      // Record the average of each adjacent pair of peaks.
      if ( abr.npeak() > 1 ) {
        for ( Index ipea=0; ipea<abr.npeak()-1; ++ipea ) {
          SampleIndex avg = (abr.peak(ipea).truncMean + abr.peak(ipea+1).truncMean)/2;
          peakAvgs.insert(avg);
        }
      }
    }
    // Find peaks in the average distribution.
    for ( SampleIndex avg : peakAvgs ) {
      m_avgBins.addSample(avg);
    }
    m_avgBins.findPeaks(10);
  }
  if ( doTree ) {
    m_ptree->Write();
    // Closing and opening the file prevents a crash in draw().
    ptreeFile->Close();
    ptreeFile = TFile::Open(fname.c_str(), "EXIST");
    m_ptree = dynamic_cast<TTree*>(ptreeFile->Get("adcdata"));
    olddir->cd();
  }
  return 0;
}

//**********************************************************************

AdcCode AdcBinarySampleReader::code(SampleIndex isam) const {
  if ( isam >= m_data.size() ) return 0.0;
  return m_data[isam];
}

//**********************************************************************

TTree* AdcBinarySampleReader::tree() {
  Name myname = "AdcBinarySampleReader::tree: ";
  if ( m_ptree == nullptr ) {
    m_doTree = true;
    read();
  }
  return m_ptree;
}

//**********************************************************************

TCanvas* AdcBinarySampleReader::draw() {
  TTree* ptree = tree();
  if ( ptree == 0 ) return nullptr;
  double xmin = 0.0;
  double xmax = nsample();
  int nx = xmax/1000;
  double yund = underflowCode();
  double yovr = overflowCode();
  double ymin = yund;
  double ymax = yovr + 1;
  double dy = 0.01*(ymax - ymin);
  ymin -= dy;
  ymax += dy;
  int ny = 100;
  ostringstream sshtitl;
  sshtitl << "ADC response for channel " << channel() << "; Tick; ADC code";
  Name htitl = sshtitl.str();
  TH2* phax = new TH2F("h2ax", htitl.c_str(), nx, xmin, xmax, ny, ymin, ymax);
  phax->SetStats(0);
  phax->GetYaxis()->SetTitleOffset(1.3);
  TCanvas* pcan = new TCanvas;
  pcan->SetRightMargin(0.06);
  phax->Draw();
  ptree->Draw("code:Entry$", "", "same");
  double ylines[2] = {yund, yovr};
  for ( double y : ylines ) {
    TLine* pline = new TLine(xmin, y, xmax, y);
    pline->SetLineStyle(3);
    pline->Draw();
  }
cout << 90 << endl;
  return pcan;
}

//**********************************************************************
