// AdcPerformanceTree.cxx

#include "AdcPerformanceTree.h"
#include <iostream>
#include "TFile.h"
#include "TTree.h"

using std::string;
using std::cout;
using std::endl;

using Index=AdcPerformanceTree::Index;

//**********************************************************************

AdcPerformanceTree::
AdcPerformanceTree(Name fname, Name tname, Name opt)
: m_status(1), m_fname(fname), m_tname(tname),
  m_pfile(nullptr), m_ptree(nullptr),
  m_pperf(new AdcVoltagePerformance),
  m_writetree(false) {
  const string myname = "AdcPerformanceTree::ctor: ";
  TDirectory* thisdir = gDirectory;
  m_pfile = TFile::Open(fname.c_str(), opt.c_str());
  m_status = 2;
  if ( m_pfile == nullptr || !m_pfile->IsOpen() ) {
    cout << myname << "ERROR: Unable to open file " << fname << endl;
    return;
  }
  m_status = 3;
  m_ptree = dynamic_cast<TTree*>(m_pfile->Get(m_tname.c_str()));
  if ( m_ptree == nullptr ) {
    cout << myname << "Creating tree " << m_tname << endl;
    m_ptree = new TTree(m_tname.c_str(), "ADC performance tree");
    m_ptree->Branch("perf", m_pperf);
    m_writetree = true;
  } else {
    m_ptree->SetBranchAddress("perf", &m_pperf);
    m_ptree->GetEntry(0);
  }
  thisdir->cd();
  m_status = 0;
}

//**********************************************************************

AdcPerformanceTree::~AdcPerformanceTree() {
  close();
  delete m_pperf;
}

//**********************************************************************

int AdcPerformanceTree::close() {
  if ( m_pfile != nullptr ) {
    TDirectory* thisdir = gDirectory;
    bool cdback = thisdir != m_pfile;
    m_pfile->cd();
    if ( m_ptree != nullptr ) {
      if ( m_writetree ) m_ptree->Write();
      m_ptree = nullptr;
    }
    m_pfile->Purge();
    m_pfile->Close();
    if ( cdback ) thisdir->cd();
    delete m_pfile;
    m_pfile = nullptr;
    m_ptree = nullptr;
  }
  return 0;
}

//**********************************************************************

Index AdcPerformanceTree::size() const {
  if ( m_ptree == nullptr ) return 0;
  return m_ptree->GetEntries();
}

//**********************************************************************

int AdcPerformanceTree::insert(const AdcVoltagePerformance& perf) {
  if ( status() ) return status();
  (*m_pperf) = perf;
  return insert();
}

//**********************************************************************

int AdcPerformanceTree::insert() {
  if ( status() ) return status();
  m_ptree->Fill();
  m_writetree = true;
  return 0;
}

//**********************************************************************

const AdcVoltagePerformance* AdcPerformanceTree::find(Index ient) const {
  if ( status() ) return nullptr;
  if ( ient >= m_ptree->GetEntries() ) return nullptr;
  m_ptree->GetEntry(ient);
  return m_pperf;
}

//**********************************************************************

const AdcVoltagePerformance* AdcPerformanceTree::find(AdcChannelId id, float vuncmax) const {
  if ( status() ) return nullptr;
  for ( Index ient=0; ient<size(); ++ient ) {
    find(ient);
    if ( m_pperf == nullptr ) continue;
    if ( m_pperf->id() == id ) {
      float vdiff = 0.0;
      if ( vuncmax > 0.0 ) {
        vdiff = fabs(m_pperf->vuncmax - vuncmax);
      }
      if ( vdiff < 1.e-5 ) return m_pperf;
    }
  }
  return nullptr;
}

//**********************************************************************

const AdcVoltagePerformance* AdcPerformanceTree::
find(Index chip, Index chan, float vuncmax) const {
  return find (AdcChannelId(chip, chan), vuncmax);
}

//**********************************************************************

const AdcVoltagePerformance* AdcPerformanceTree::
findNext(Index& ient, Index chip, Index chan) const {
  if ( status() ) return nullptr;
  for ( ; ient<size(); ++ient ) {
    find(ient);
    if ( m_pperf == nullptr ) continue;
    if ( m_pperf->chip == chip && m_pperf->chan == chan ) return m_pperf;
  }
  find(0);
  return nullptr;
}

//**********************************************************************

const AdcVoltagePerformance* AdcPerformanceTree::
findSample(Index& ient, Name sampleName) const {
  if ( status() ) return nullptr;
  for ( ; ient<size(); ++ient ) {
    find(ient);
    if ( m_pperf == nullptr ) continue;
    if ( m_pperf->sampleName == sampleName ) return m_pperf;
  }
  find(0);
  return nullptr;
}

//**********************************************************************
