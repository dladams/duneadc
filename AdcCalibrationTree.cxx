// AdcCalibrationTree.cxx

#include "AdcCalibrationTree.h"
#include <iostream>
#include <iomanip>
#include "TFile.h"
#include "TTree.h"

using std::string;
using std::cout;
using std::endl;
using std::setw;

using Index = AdcCalibrationTree::Index;

//**********************************************************************

AdcCalibrationTree::
AdcCalibrationTree(Name fname, Name tname, Name opt)
: m_status(11), m_fname(fname), m_tname(tname),
  m_pfile(nullptr), m_ptree(nullptr),
  m_pdat(&m_cal.data()),
  m_modified(false) {
  const string myname = "AdcCalibrationTree::ctor: ";
  TDirectory* thisdir = gDirectory;
  m_pfile = TFile::Open(fname.c_str(), opt.c_str());
  m_status = 12;
  if ( m_pfile == nullptr || !m_pfile->IsOpen() ) {
    cout << myname << "ERROR: Unable to open file " << fname << endl;
    return;
  }
  m_status = 13;
  m_ptree = dynamic_cast<TTree*>(m_pfile->Get(m_tname.c_str()));
  if ( m_ptree == nullptr ) {
    cout << myname << "Creating tree " << m_tname << endl;
    m_ptree = new TTree(m_tname.c_str(), "ADC calibration tree");
    m_ptree->Branch("cal", m_pdat);
  } else {
    m_ptree->SetBranchAddress("cal", &m_pdat);
    m_ptree->GetEntry(0);
  }
  thisdir->cd();
  m_status = 0;
  m_baseCalibName = fname;
  if ( fname.substr(0, 6) == "calib_" ) m_baseCalibName = m_baseCalibName.substr(6);
  string::size_type ipos = m_baseCalibName.find(".root");
  if ( ipos != string::npos ) m_baseCalibName = m_baseCalibName.substr(0, ipos);
}

//**********************************************************************

AdcCalibrationTree::~AdcCalibrationTree() {
  close();
}

//**********************************************************************

int AdcCalibrationTree::close() {
  string myname = "AdcCalibrationTree::close: ";
  if ( m_pfile != nullptr ) {
    cout << myname << "Closing " << m_pfile->GetName() << endl;
    TDirectory* thisdir = gDirectory;
    bool cdback = thisdir != m_pfile;
    m_pfile->cd();
    if ( m_ptree != nullptr ) {
      if ( m_modified ) m_ptree->Write();
      m_ptree = nullptr;
    }
    if ( m_modified ) m_pfile->Purge();
    m_pfile->Close();
    if ( cdback ) thisdir->cd();
    delete m_pfile;
    m_pfile = nullptr;
    m_ptree = nullptr;
  }
  return 0;
}

//**********************************************************************

Index AdcCalibrationTree::size() const {
  if ( m_ptree == nullptr ) return 0;
  return m_ptree->GetEntries();
}

//**********************************************************************

int AdcCalibrationTree::
insert(const AdcTreeChannelCalibration& cal, bool dup) {
  Index ient;
  if ( !dup && find(cal.chip(), cal.channel(), cal.time(), ient) != nullptr ) {
    return 1;
  }
  if ( status() ) return status();
  m_cal = cal;
  return insert();
}

//**********************************************************************

int AdcCalibrationTree::insert() {
  if ( status() ) return status();
  m_ptree->Fill();
  m_modified = true;
  return 0;
}

//**********************************************************************

const AdcTreeChannelCalibration* AdcCalibrationTree::
find(Index ient) const {
  if ( status() ) return nullptr;
  if ( ient >= m_ptree->GetEntries() ) return nullptr;
  m_ptree->GetEntry(ient);
  ostringstream sscnam;
  sscnam << m_baseCalibName
         << "_chip" << m_cal.chip()
         << "_chan" << m_cal.channel()
         << "_time" << m_cal.time();
  string scnam = sscnam.str();
  m_cal.setName(scnam);
  return &m_cal;
}

//**********************************************************************

const AdcTreeChannelCalibration* AdcCalibrationTree::
find(AdcChannelId aid, Index& ient) const {
  return find(aid.chip, aid.chan, ient);
}

//**********************************************************************

const AdcTreeChannelCalibration* AdcCalibrationTree::
find(Index chip, Index chan, Index& ient) const {
  if ( status() ) return nullptr;
  Index nent = m_ptree->GetEntries();
  for ( ; ient<nent; ++ient ) {
    const AdcTreeChannelCalibration* pcal = find(ient);
    if ( pcal->chip() == chip && pcal->channel() == chan ) return pcal;
  }
  return find(ient);
}

//**********************************************************************

const AdcTreeChannelCalibration* AdcCalibrationTree::
find(Index chip, Index chan, AdcTime time, Index& ient) const {
  if ( status() ) return nullptr;
  Index nent = m_ptree->GetEntries();
  for ( ; ient<nent; ++ient ) {
    const AdcTreeChannelCalibration* pcal = find(ient);
    if ( pcal->chip() == chip &&
         pcal->channel() == chan &&
         pcal->time() == time ) return pcal;
  }
  return find(ient);
}

//**********************************************************************

AdcTreeChannelCalibration AdcCalibrationTree::
get(Index ient) const {
  const AdcTreeChannelCalibration* pcal = find(ient);
  if ( pcal == nullptr ) return std::move(AdcTreeChannelCalibration());
  return std::move(AdcTreeChannelCalibration(*pcal)) ;
}

//**********************************************************************

AdcTreeChannelCalibration AdcCalibrationTree::
get(AdcChannelId aid, Index& ient) const {
  const AdcTreeChannelCalibration* pcal = find(aid, ient);
  if ( pcal == nullptr ) return std::move(AdcTreeChannelCalibration());
  return std::move(AdcTreeChannelCalibration(*pcal)) ;
}

//**********************************************************************

AdcTreeChannelCalibration AdcCalibrationTree::
get(Index chip, Index chan, Index& ient) const {
  const AdcTreeChannelCalibration* pcal = find(chip, chan, ient);
  if ( pcal == nullptr ) return std::move(AdcTreeChannelCalibration());
  return std::move(AdcTreeChannelCalibration(*pcal)) ;
}

//**********************************************************************

AdcTreeChannelCalibration AdcCalibrationTree::
get(Index chip, Index chan, AdcTime time, Index& ient) const {
  const AdcTreeChannelCalibration* pcal = find(chip, chan, time, ient);
  if ( pcal == nullptr ) return std::move(AdcTreeChannelCalibration());
  return std::move(AdcTreeChannelCalibration(*pcal)) ;
}

//**********************************************************************

void AdcCalibrationTree::print(Index maxent) const {
  Index nent = maxent<size() ? maxent : size();
  Index went = 4 + log10(nent);
  Index wcon = 6;
  Index wcha = 5;
  Index wtim = 11;
  cout << "Tree has " << size() << " " << (size()==1 ? "entry" : "entries")
       << (size() == 0 ? "." : ":") << endl;
  cout << setw(went+wcon) << "Chip" << " Chan       Time" << endl;
  for ( Index ient  =0; ient<nent; ++ient ) {
    const AdcTreeChannelCalibration* pcal = find(ient);
    TDatime dt(pcal->time());
    cout << setw(went) << ient
         << setw(wcon) << pcal->chip()
         << setw(wcha) << pcal->channel()
         << setw(wtim) << pcal->time()
         << "  " << dt.AsString()
         << endl;
  }
}

//**********************************************************************
