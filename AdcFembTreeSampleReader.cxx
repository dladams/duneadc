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

using std::ostringstream;

using Name = AdcFembTreeSampleReader::Name;

//**********************************************************************

AdcFembTreeSampleReader::AdcFembTreeSampleReader(Name fname, Index chan)
: m_fname(fname), m_channel(chan),
  m_pinTree(nullptr),
  m_pdata(nullptr), m_pvin(nullptr) {
  const string myname = "AdcFembTreeSampleReader::ctor: ";
  const int dbg = 0;
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
  int rstat = inputTree()->SetBranchAddress("wf", &m_pdata);
  if ( rstat != 0 ) cout << myname << "Set wf branch address returned " << rstat << endl;
  rstat = inputTree()->SetBranchAddress("voltage", &m_pvin);
  if ( rstat != 0 ) cout << myname << "Set voltage branch address returned " << rstat << endl;
  rstat = inputTree()->GetEntry(channel());
  if ( rstat != 0 ) cout << myname << "Set channel returned " <<rstat << endl;
  if ( dbg ) {
    cout << myname << "Data address: " << m_pdata << endl;
    cout << myname << "Data size: " << m_pdata->size() << endl;
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
  return m_pdata->size();
}

//**********************************************************************

AdcCode AdcFembTreeSampleReader::code(SampleIndex isam) const {
  if ( m_pdata == nullptr ) return 0;
  auto data = *m_pdata;
  if ( data.size() <= isam ) return 0;
  return data[isam];
}

//**********************************************************************

double AdcFembTreeSampleReader::vin(SampleIndex isam) const {
  if ( m_pvin == nullptr ) return 0;
  auto vin = *m_pvin;
  if ( vin.size() <= isam ) return 0;
  return 1000*vin[isam];
}

//**********************************************************************
