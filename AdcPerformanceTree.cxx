// AdcPerformanceTree.cxx

#include "AdcPerformanceTree.h"
#include <iostream>
#include "TFile.h"
#include "TTree.h"

using std::string;
using std::cout;
using std::endl;

//**********************************************************************

AdcPerformanceTree::
AdcPerformanceTree(Name fname, Name tname, Name opt)
: m_status(1), m_fname(fname), m_tname(tname),
  m_pfile(nullptr), m_ptree(nullptr),
  m_pperf(new AdcVoltagePerformance) {
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
      m_ptree->Write();
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

int AdcPerformanceTree::insert(const AdcVoltagePerformance& perf) {
  if ( status() ) return status();
  (*m_pperf) = perf;
  return insert();
}

//**********************************************************************

int AdcPerformanceTree::insert() {
  if ( status() ) return status();
  m_ptree->Fill();
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

const AdcVoltagePerformance* AdcPerformanceTree::find(AdcChannelId, float) const {
  if ( status() ) return nullptr;
  return nullptr;
}

//**********************************************************************
