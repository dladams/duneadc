// AdcFembTreeSampleReader.cxx

#include "AdcFembTreeSampleReader.h"
#include "SampleFunction.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "TDirectory.h"
#include "TFile.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TLine.h"
#include "TDatime.h"

using std::ostringstream;

using Name = AdcFembTreeSampleReader::Name;

//**********************************************************************

AdcFembTreeSampleReader::
AdcFembTreeSampleReader(Name fname, Index chan, Name ssam,
                        SampleIndex isam0, SampleIndex nsam)
: AdcFembTreeSampleReader(fname, chan, -1, ssam, isam0, nsam) { }

//**********************************************************************

AdcFembTreeSampleReader::
AdcFembTreeSampleReader(Name fname, Index chan, int event, Name ssam,
                        SampleIndex isam0, SampleIndex nsam)
: AdcFembTreeSampleReader(fname, chan, -1, -1, ssam, isam0, nsam) { }

//**********************************************************************

AdcFembTreeSampleReader::
AdcFembTreeSampleReader(Name fname, Index chan, int event, int chanoff, Name ssam,
                        SampleIndex isam0, SampleIndex nsam)
: m_fname(fname), m_channel(chan), m_sample(ssam),
  m_isam0(isam0), m_nsam(nsam),
  m_pinTree(nullptr), 
  m_pdata(nullptr), m_pvin(nullptr),
  m_event(event), m_chanoff(chanoff) {
  const string myname = "AdcFembTreeSampleReader::ctor: ";
  const int dbg = 1;
  // Find DS name.
  string::size_type ipos = ssam.find("_");
  m_dsname = ssam.substr(0, ipos);
  // Find chip ID.
  string ssamrem = ssam;
  Index mappedChip = 0;
  bool getChipFromTree = true;
  string::size_type jpos = ssam.find(":");
  bool chipIsMapped = jpos != string::npos;  // Chip mapped to ID diffeerent from that in file name.
  if ( chipIsMapped ) {
    ssamrem = ssam.substr(0,jpos);
    string schprem = ssam.substr(jpos+1);
    if ( schprem.substr(0,4) != "chip" ) {
      cout << myname << "ERROR: " << "Invalid chip map syntax for sample " << ssam << endl;
      return;
    }
    string schp = schprem.substr(4);
    bool isD = schp[0] = 'D';
    if ( isD ) schp = schp.substr(1);
    while ( schp[0] == '0' ) schp = schp.substr(1);
    m_chip = 0;
    if ( schp.size() ) {
      istringstream sschp(schp);
      sschp >> m_chip;
    }
    if ( isD ) m_chip += 10000;
    getChipFromTree = false;
  } else if ( ipos != string::npos ) {
    string srem = ssam.substr(ipos+1);
    if ( srem.substr(0,5) == "chipD" ) {
      getChipFromTree = false;
      string schp = srem.substr(5);
      ipos = schp.find("_");
      if ( ipos != string::npos ) schp = schp.substr(0, ipos);
      ipos = 0;
      while ( schp.size() ) {
        if ( schp[0] == '0' ) schp = schp.substr(1);
        else break;
      }
      if ( schp.size() ) {
        istringstream sschp(schp);
        sschp >> m_chip;
        m_chip += 10000;
      }
    }
    if ( m_chip == 0 ) {
      cout << myname << "ERROR: " << "Unable to deduce chip number from sample name " << ssam << endl;
    }
  }
  // Open input file.
  std::ifstream* pin = new std::ifstream;
  TDirectory* pdirIn = gDirectory;
  m_pinFile = TFile::Open(inputFileName().c_str());
  pdirIn->cd();
  if ( inputFile() == nullptr || !inputFile()->IsOpen() ) {
    delete m_pinFile;
    m_pinFile = nullptr;
    cout << myname << "ERROR: " << "Unable to open input file: " << inputFileName() << endl;
    return;
  }
  string tname = "femb_wfdata";
  m_pinTree = dynamic_cast<TTree*>(inputFile()->Get(tname.c_str()));
  if ( inputTree() == nullptr ) {
    cout << myname << "ERROR: " << "Unable to find input tree " << tname << endl;
    return;
  }
  // Fetch the waveform.
  int rstat = inputTree()->SetBranchAddress("wf", &m_pdata);
  if ( rstat != 0 ) cout << myname << "Set wf branch address returned " << rstat << endl;
  // If an event is specified (i.e. > 0), then we expect to find a branch "subrun" holding
  // that value and a branch "chan" with the channel number.
  if ( m_event >= 0 ) {
    rstat = inputTree()->SetBranchAddress("subrun", &m_eventInTree);
    if ( rstat != 0 ) {
      cout << myname << "ERROR: Set subrun branch address returned " << rstat << endl;
      return;
    }
    if ( m_chanoff < 0 ) m_chanoff = 0;  // Also require the branch "chan".
  }
  // If a channel offset is specified, then we expect to find the branch "chan".
  if ( m_chanoff >= 0 ) {
    rstat = inputTree()->SetBranchAddress("chan", &m_chanInTree);
    if ( rstat != 0 ) {
      cout << myname << "ERROR: Set subrun branch address returned " << rstat << endl;
      return;
    }
  }
  rstat = inputTree()->SetBranchAddress("voltage", &m_pvin);
  if ( rstat != 0 ) cout << myname << "Set voltage branch address returned " << rstat << endl;
  Index nbyte = 0;
  if ( m_chanoff >= 0 ) {
    UShort_t treeChan = m_chanoff + chan;
    UShort_t treeEvent = m_event;
    for ( unsigned int ient=0; ient<inputTree()->GetEntries(); ++ient ) {
      nbyte = inputTree()->GetEntry(ient);
      if ( m_eventInTree == treeEvent && m_chanInTree == treeChan ) break;
    }
  } else {
    nbyte = inputTree()->GetEntry(channel());
  }
  // Fetch the input voltage.
  if ( nbyte == 0 ) {
    cout << myname << "ERROR: Tree has no data for channel " << channel() << endl;
    if ( m_chanoff >= 0 ) cout << myname << "       for event " << m_event << " and channel offset " << m_chanoff << endl;
    return;
  }
  if ( dbg ) cout << myname << "Read size for channel " << channel() << ": " << nbyte << " bytes." << endl;
  SampleIndex nsamRaw = m_pdata->size();
  SampleIndex nsamOff = nsamRaw > m_isam0 ? nsamRaw - m_isam0 : 0;
  // Set the sample count tnot to extend beyond the end of the input data.
  if ( m_nsam == 0 || nsamOff < m_nsam ) m_nsam = nsamOff;
  // Fetch metadata.
  string mdtname = "metadata";
  TTree* pmdt = dynamic_cast<TTree*>(inputFile()->Get(mdtname.c_str()));
  if ( pmdt == nullptr ) {
    cout << myname << "ERROR: " << "Metadata tree not found: " << mdtname << endl;
  } else {
    ULong64_t date = 0;
    ULong64_t iChip = badChip();
    Long64_t t_adcSerial;
    Long64_t t_feSerial;
    Float_t t_sampleRate = 0.0;
    Float t_funcAmp;
    Float t_funcOffset;
    Float t_funcFreq;
    pmdt->SetBranchAddress("date",       &date);
    pmdt->SetBranchAddress("iChip",      &iChip);
    if ( getChipFromTree ) {
      pmdt->SetBranchAddress("adcSerial",  &t_adcSerial);
    }
    pmdt->SetBranchAddress("feSerial",   &t_feSerial);
    pmdt->SetBranchAddress("sampleRate", &t_sampleRate);
    pmdt->SetBranchAddress("funcAmp",    &t_funcAmp);
    pmdt->SetBranchAddress("funcOffset", &t_funcOffset);
    pmdt->SetBranchAddress("funcFreq",   &t_funcFreq);
    pmdt->GetEntry(0);
    // Convert date from yyyymmddhhmm to unix time.
    {
      ULong64_t remdate = date;
      int sec   = 0;
      int min   = date % 100;
      int hour  = (remdate/=100) % 100;
      int day   = (remdate/=100) % 100;
      int month = (remdate/=100) % 100;
      int year  = (remdate/=100);
      TDatime tda(year, month, day, hour, min, sec);
      m_time = tda.Convert();
    }
    if ( getChipFromTree ) {
      m_chip = t_adcSerial;
      m_adcSerial = t_adcSerial;
    } else {
      m_adcSerial = 0;
    }
    m_fsamp = t_sampleRate;
    m_feSerial = t_feSerial;
    m_vinAmp = 1000.0*t_funcAmp;
    m_vinOffset = 1000.0*t_funcOffset;
    m_vinFreq = t_funcFreq;
  }
  // Debug display.
  if ( dbg ) {
    cout << myname << "     Dataset: " << dataset() << endl;
    cout << myname << "      Sample: " << sample() << endl;
    cout << myname << "   Data size: " << m_pdata->size() << endl;
    cout << myname << "Data address: " << m_pdata << endl;
    cout << myname << "        Chip: " << chip() << endl;
    cout << myname << "     Channel: " << channel() << endl;
    cout << myname << "        Time: " << TDatime(m_time).AsString() << endl;
    cout << myname << " Sample rate: " << samplingFrequency() << " Hz" << endl;
    cout << myname << "  ADC serial: " << adcSerial() << endl;
    cout << myname << "   FE serial: " << feSerial() << endl;
    cout << myname << "     Vin min: " << vinMin() << " mV" << endl;
    cout << myname << "     Vin max: " << vinMax() << " mV" << endl;
    cout << myname << "    Vin freq: " << vinFreq() << " Hz" << endl;
  }
}

//**********************************************************************

AdcFembTreeSampleReader::~AdcFembTreeSampleReader() {
  delete m_pinFile;
  m_pinFile = nullptr;
}

//**********************************************************************

SampleIndex AdcFembTreeSampleReader::nsample() const {
  if ( m_pdata == nullptr ) return 0;
  return m_nsam;
}

//**********************************************************************

AdcCode AdcFembTreeSampleReader::code(SampleIndex isam) const {
  if ( m_pdata == nullptr ) return 0;
  auto& data = *m_pdata;
  SampleIndex ksam = m_isam0 + isam;
  if ( data.size() <= ksam ) return 0;
  return data[ksam];
}

//**********************************************************************

double AdcFembTreeSampleReader::vin(SampleIndex isam, double* pdvds) const {
  if ( m_samfun != nullptr ) return m_samfun->value(isam, pdvds);
  if ( m_pvin == nullptr ) return 0;
  auto vin = *m_pvin;
  SampleIndex ksam = m_isam0 + isam;
  if ( vin.size() <= ksam ) return 0;
  return 1000*vin[ksam];
}

//**********************************************************************
