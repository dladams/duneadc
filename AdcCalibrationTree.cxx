// AdcCalibrationTree.cxx

#include "AdcCalibrationTree.h"
#include "TFile.h"
#include "TTree.h"

//**********************************************************************

AdcCalibrationTree::
AdcCalibrationTree(Name fname, Name tname, Name opt)
: m_status(1), m_fname(fname), m_tname(tname), m_pfile(nullptr), m_ptree(nullptr) {
  TDirectory* thisdir = gDirectory;
  m_pfile = TFile::Open(fname.c_str(), opt.c_str());
  thisdir->cd();
  m_status = 2;
  if ( m_pfile == nullptr || !m_pfile->IsOpen() ) return;
  m_status = 3;
  if ( m_tname == "" ) m_tname = "adccaltree";
  TObject* pobj = m_pfile->Get(tname.c_str());
  m_ptree = dynamic_cast<TTree*>(pobj);
  if ( pobj != nullptr && m_ptree == nullptr ) return;
  if ( m_ptree == nullptr ) {
    m_status = 5;
    if ( maketree() ) return;
  }
  m_status = 0;
}

//**********************************************************************

int AdcCalibrationTree::insert(const AdcChannelCalibration cal) {
  if ( status() ) return status();
  m_buff = cal;
  return insert();
}

//**********************************************************************

int AdcCalibrationTree::insert() {
  if ( status() ) return status();
  m_ptree->Fill();
  return 0;
}

//**********************************************************************

const AdcChannelCalibration* AdcCalibrationTree::find(Index ient) const {
  if ( status() ) return nullptr;
  if ( ient >= m_ptree->GetEntries() ) return nullptr;
  m_ptree->GetEntry(ient);
  return &m_buff;
}

//**********************************************************************

const AdcChannelCalibration* AdcCalibrationTree::find(AdcChannelId) const {
  if ( status() ) return nullptr;
  return nullptr;
}

//**********************************************************************

int AdcCalibrationTree::maketree() {
  if ( m_ptree != nullptr ) return m_status = 11;
  if ( m_pfile == nullptr ) return m_status = 12;
  if ( ! m_pfile->IsOpen() ) return m_status = 13;
  TDirectory* thisdir = gDirectory;
  m_pfile->cd();
  m_ptree = new TTree(m_tname.c_str(), "ADC calibration tree");
  thisdir->cd();
  m_ptree->Branch("cal", &m_buff);
  m_ptree->Print();
  return 0;
}

//**********************************************************************
