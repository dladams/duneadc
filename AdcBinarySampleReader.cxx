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

//**********************************************************************
// Subclass definitions.
//**********************************************************************

void AdcBinarySampleReader::SampleRange::print() const {
  cout << "start=" << begin() << ", size=" << size() << ", state="
       << stateName(state()) << endl;
}

//**********************************************************************
// Class definitions.
//**********************************************************************

Name AdcBinarySampleReader::stateName(State state) {
  if ( state == UNKNOWN ) return "UNKNOWN";
  if ( state == UNDER ) return "UNDER";
  if ( state == RISING ) return "RISING";
  if ( state == OVER ) return "OVER";
  if ( state == FALLING ) return "FALLING";
  return "StateError";
}

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

int AdcBinarySampleReader::readBorders() {
  const Name myname = "AdcBinarySampleReader::readBorders: ";
  if ( m_pin == nullptr ) return 1;
  if ( m_haveReadFile ) return 2;
  istream& fin = *m_pin;
  if ( sizeof(short) != 2 ) return 3;
  if ( sizeof(SampleIndex) != 8 ) return 4;
  SampleIndex l1 = 1;
  const int nsambuf = l1 << 19;
  AdcCode buff[nsambuf];
  fin.seekg(0, fin.end);
  m_nsample = fin.tellg()/2;
  fin.seekg(0, fin.beg);
  cout << myname << "  # samples: " << setw(10) << nsample() << endl;
  cout << myname << "Buffer size: " << setw(10) << nsambuf << endl;
  // Loop over block reads.
  SampleIndex ksam = 0;     // sample position in the stream
  SampleIndex isam = 0;     // sample position in the buffer.
  State state = UNKNOWN;    // Current stream state.
  SampleRange range;        // Current range
  SampleIndex nsamGood = 0; // # contiguous good samples
  SampleIndex count = 0;    // Counter to check each sample is read.
  bool foundStartingFence = false; // Flag indicating initial block of good samples is found.
  while ( true ) {
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
      ++count;
      AdcCode chancode = buff[isam];
      AdcCode code = chancode&chanMask();
      Index chan = chancode>>chanShift();
      if ( ksam == 0 ) {
        m_channel = chan;
        cout << "Channel number is " << channel() << endl;
      }
      if ( chan != m_channel ) return 5;
      bool sampleIsUnder = code == underflowCode();
      bool sampleIsOver = code == overflowCode();
      bool sampleIsGood = code > underflowCode() && code < overflowCode();
      bool streamIsUnder = state == UNDER;
      bool streamIsOver = state == OVER;
      if ( !sampleIsUnder && !sampleIsOver && !sampleIsGood ) {
        cout << "Unexpected ADC code: " << code << " for sample " << ksam << endl;
        return 6;
      }
      // Still looking for starting fence.
      if ( ! foundStartingFence ) {
        if ( sampleIsGood ) {
          ++nsamGood;
          if ( nsamGood >= fence() ) {
            SampleRange tmprange(ksam+1-fence(), ksam+1);
            cout << myname << "Found starting fence: ";
            tmprange.print();
            cout << endl;
            foundStartingFence = true;
          }
        } else {
          nsamGood = 0;
        }
      // Stream is inside a candidate underflow or overflow region and we are looking
      // for the trailing fence. Add range once that is found.
      } else if ( streamIsUnder || streamIsOver ) {
        if ( sampleIsGood ) {
          ++nsamGood;
          if ( nsamGood >= fence() ) {
            cout << myname << "Ending " << stateName(state) << " at " << ksam << endl;
            m_borders.push_back(range);
            range.print();
            borders().back().print();
            range.reset();
            if ( state == UNDER ) state = RISING;
            if ( state ==  OVER ) state = FALLING;
          }
        } else {
          if ( streamIsUnder && !sampleIsUnder ) return 7;
          if ( streamIsOver && !sampleIsOver ) return 8;
          nsamGood = 0;
          range.setEnd(ksam + 1);
        }
      // Stream is between underflow and overflow regions.
      // Look for the next such region flagged by one such sample.
      } else {
        if ( sampleIsUnder ) {
          if ( nsamGood < fence() ) return 9;
          cout << myname << "Starting underflow at " << ksam << endl;
          range.set(ksam, ksam+1, UNDER);
          range.print();
          state = UNDER;
          nsamGood = 0;
        } else if ( sampleIsOver ) {
          if ( nsamGood < fence() ) return 10;
          cout << myname << "Starting overflow at " << ksam << endl;
          range.set(ksam, ksam+1, OVER);
          range.print();
          state = OVER;
          nsamGood = 0;
        }
      }
    }
  }
  if ( count != nsample() ) {
    cout << "Error counting samples." << endl;
  }
  if ( ksam != nsample() ) {
    cout << "Error indexing samples." << endl;
  }
  m_haveReadFile = true;
  return 0;
}

//**********************************************************************

int AdcBinarySampleReader::read(AdcCodeVector* pdat) {
  Name myname = "AdcBinarySampleReader::read: ";
  bool doBins = m_doBins && m_abrs.size() == 0;
  bool doTree = m_doTree && m_ptree == nullptr;
  bool doData = pdat != nullptr;
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
  if ( doBins ) {
    m_abrs.resize(4096);
  }
  if ( doData ) {
    pdat->clear();
    pdat->resize(maxCount);
  }
  // Loop over block reads.
  SampleIndex l1 = 1;
  const int nsambuf = l1 << 19;
  AdcCode buff[nsambuf];
  cout << myname << "Buffer size: " << setw(10) << nsambuf << endl;
  SampleIndex ksam = 0;     // sample position in the stream
  SampleIndex isam = 0;     // sample position in the buffer.
  SampleIndex count = 0;    // Counter to check each sample is read.
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
      if ( m_nDump > 0 ) {
        cout << setw(10) << count << ": channel= " << setw(2) << chan << ", code="
             << setw(4) << code << endl;
      } else if ( ksam == 0 ) {
        cout << "Channel number is " << channel() << endl;
      }
      if ( doTree ) m_ptree->Fill();
      if ( doBins ) {
        if ( code >= m_abrs.size() ) m_abrs.resize(code+1);
        m_abrs[code].addSample(ksam);
      }
      if ( doData ) (*pdat)[ksam] = code;
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
