// AdcBinarySampleReader.cxx

#include "AdcBinarySampleReader.h"
#include "SampleFunction.h"
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

AdcBinarySampleReader::AdcBinarySampleReader(Name fname)
: m_underflowCode(0),
  m_overflowCode(4095),
  m_chanMask(0xfff),
  m_chanShift(12),
  m_ownStream(true),
  m_pin(nullptr),
  m_fname(fname),
  m_nsample(0),
  m_haveReadFile(false),
  m_ptree(nullptr) {
  const string myname = "AdcBinarySampleReader::ctor: ";
  std::ifstream* pin = new std::ifstream;
  m_pin = pin;
  pin->open(fname.c_str(), ios::binary);
  cout << myname << "       File: " << fname << endl;
}

//**********************************************************************

AdcBinarySampleReader::
AdcBinarySampleReader(Name fname, Name a_sample, Index a_chip, Name a_chipLabel,
                      Index icha, double fsamp)
: AdcBinarySampleReader(fname) {
  const string myname = "AdcBinarySampleReader::ctor: ";
  m_sample = a_sample;
  string::size_type ipos = m_sample.find("_");
  m_dsname = m_sample.substr(0, ipos);
  m_chip = a_chip;
  m_chipLabel= a_chipLabel;
  m_channel = icha;
  m_fsamp = fsamp;
  cout << myname << "    Dataset: " << dataset() << endl;
  cout << myname << "       Chip: " << chip() << endl;
  cout << myname << " Chip label: " << chipLabel() << endl;
  cout << myname << "    Channel: " << channel() << endl;
}

//**********************************************************************

AdcBinarySampleReader::~AdcBinarySampleReader() {
  if ( m_ownStream ) delete m_pin;
  m_pin = nullptr;
}

//**********************************************************************

SampleIndex AdcBinarySampleReader::nsample() const {
  if ( ! m_haveReadFile ) read();
  return m_nsample;
}

//**********************************************************************

int AdcBinarySampleReader::read() const {
  Name myname = "AdcBinarySampleReader::read: ";
  m_haveReadFile = true;
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
  if ( !doTree && !doData ) maxCount = m_nDump;
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
  SampleIndex nbadChan = 0;
  SampleIndex ngoodChan = 0;
  SampleIndex nskipChan = 0;
  SampleIndex maxwarn = 1000;
  bool skipping = true;
  while ( count < maxCount ) {
    fin.seekg(2*ksam);
    SampleIndex ksamNext = ksam + nsambuf;
    if ( ksamNext > nsample() ) ksamNext = nsample();
    SampleIndex nsamRead = ksamNext - ksam;
    if ( nsamRead == 0 ) break;
    fin.read((char*)buff, 2*nsamRead);
    cout << myname << "Read block at sample " << setw(8) << ksam << ": " << setw(6) << (buff[0]&chanMask())
         << " (size = " << nsamRead << ")" << endl;
    SampleIndex isamMax = isam + nsamRead;
    for ( SampleIndex isam=0; isam<isamMax; ++isam ) {
      if ( count >= maxCount ) break;
      AdcCode chancode = buff[isam];
      code = chancode&chanMask();
      Index chan = chancode>>chanShift();
      if ( m_channel == badChannel() ) m_channel = chan;
      if ( chan != channel() ) {
        // Skip bad channels at start of stream.
        if ( skipping ) {
          ++nskipChan;
          --m_nsample;
          if ( isamMax ) --isamMax;
          continue;
        }
        ++nbadChan;
        if ( nbadChan < maxwarn ) {
          cout << myname << "WARNING: Data[" << isam << "] channel is not consistent: "
               << chan << " != " << channel() << endl;
        }
      } else {
        skipping = false;
        ++ngoodChan;
      }
      if ( m_nDump > ndumped ) {
        cout << myname << setw(10) << count << ": channel= " << setw(2) << chan << ", code="
             << setw(4) << code << endl;
         ++ndumped;
      } else if ( ksam == 0 ) {
        cout << myname << "Channel number is " << channel() << endl;
      }
      if ( doTree ) m_ptree->Fill();
      if ( doData ) m_data[ksam] = code;
      ++ksam;
      ++count;
    }
  }
  cout << myname << "# skipped channels: " << nskipChan << endl;
  if ( nbadChan ) {
    cout << myname << "WARNING: # ticks with wrong channel is " << nbadChan << endl;
    cout << myname << "WARNING: # ticks with right channel is " << ngoodChan << endl;
  }
  if ( doTree ) {
    if ( count != nsample() ) {
      cout << myname << "WARNING: Error counting samples: " << count << " != " << nsample() << endl;
    }
  }
  SampleIndex ksamExp = count + nskipChan;
  if ( ksam != ksamExp ) {
    cout << myname << "ERROR: Sample index and count are inconsistent: " << ksam << " != "
         << count << " + " << nskipChan << endl;
  }
  if ( doTree ) {
    m_ptree->Write();
    // Closing and opening the file prevents a crash in draw().
    ptreeFile->Close();
    ptreeFile = TFile::Open(fname.c_str(), "EXIST");
    m_ptree = dynamic_cast<TTree*>(ptreeFile->Get("adcdata"));
    olddir->cd();
  }
  cout << myname << " Read complete: # samples is " << nsample() << endl;
  return 0;
}

//**********************************************************************

AdcCode AdcBinarySampleReader::code(SampleIndex isam) const {
  if ( m_data.size() == 0 ) {
    m_doData = true;
    read();
  }
  if ( isam >= m_data.size() ) return 0.0;
  return m_data[isam];
}

//**********************************************************************

double AdcBinarySampleReader::vin(SampleIndex isam) const {
  if ( m_samfun == nullptr ) return 0.0;
  return m_samfun->value(isam);
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
