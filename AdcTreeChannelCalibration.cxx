// AdcTreeChannelCalibration.cxx

#include "AdcTreeChannelCalibration.h"
#include "AdcSampleFinder.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>

using std::cout;
using std::endl;
using Name = std::string;
using CalibMap = std::map<AdcChannelId, const AdcTreeChannelCalibration*>;
using CalibMapMap = std::map<Name, CalibMap>;

//**********************************************************************
// Subclass methods.
//**********************************************************************

AdcTreeChannelCalibrationData::
AdcTreeChannelCalibrationData(ShortIndex achip, ShortIndex achan, AdcTime atime,
                              Float again, Float aoffset,
                              const FloatVector& acalMeans,
                              const FloatVector& acalRmss,
                              const ShortIndexVector& acalCounts)
: chip(achip), chan(achan), time(atime), gain(again), offset(aoffset),
  calMeans(acalMeans), calRmss(acalRmss), calCounts(acalCounts) { }

//**********************************************************************

AdcTreeChannelCalibrationData::
AdcTreeChannelCalibrationData(ShortIndex achip, ShortIndex achan, AdcTime atime,
                              Float again, Float aoffset)
: chip(achip), chan(achan), time(atime), gain(again), offset(aoffset) { }

//**********************************************************************
// Static class methods.
//**********************************************************************

const AdcTreeChannelCalibration*
AdcTreeChannelCalibration::find(std::string dataset, AdcChannelId aid) {
  static CalibMapMap calibmaps;
  const Name myname = "AdcTreeChannelCalibration::find: ";
  CalibMap& calibmap = calibmaps[dataset];
  CalibMap::iterator icm = calibmap.find(aid);
  if ( icm != calibmap.end() ) return icm->second;
  const AdcTreeChannelCalibration*& pcalout = calibmap[aid];
  pcalout = nullptr;
  Name fname = "calib_" + dataset + ".root";
  TFile* pfile = TFile::Open(fname.c_str());
  if ( pfile==nullptr || !pfile->IsOpen() ) {
    string dir = AdcSampleFinder::defaultTopdir() + "/calib";
    string longfname = dir + "/" + fname;
    delete pfile;
    pfile = TFile::Open(longfname.c_str());
    if ( pfile==nullptr || !pfile->IsOpen() ) {
      cout << myname << "Unable to find file " << fname << endl;
      cout << myname << "Please copy file to pwd or " << dir << endl;
      return nullptr;
    }
  }
  TTree* ptree = dynamic_cast<TTree*>(pfile->Get("adccalib"));
  if ( ptree==nullptr ) {
    cout << myname << "Unable to find tree adccalib in file " << fname << endl;
    return nullptr;
  }
  const AdcTreeChannelCalibrationData* pdat = new AdcTreeChannelCalibrationData;
  ptree->SetBranchAddress("cal", &pdat);
  for ( unsigned int ient=0; ient<ptree->GetEntries(); ++ient ) {
    ptree->GetEntry(ient);
    if ( pdat->chip == aid.chip && pdat->chan == aid.chan ) {
      pcalout = new AdcTreeChannelCalibration(*pdat);
      break;
    }
  }
  delete pdat;
  pfile->Close();
  delete pfile;
  return pcalout;
}

//**********************************************************************

const AdcTreeChannelCalibration*
AdcTreeChannelCalibration::find(std::string dataset, ShortIndex chip, ShortIndex chan) {
  return find(dataset, AdcChannelId(chip, chan));
}

//**********************************************************************
// Class ctors, copy and dtor.
//**********************************************************************

AdcTreeChannelCalibration::AdcTreeChannelCalibration() { }

//**********************************************************************

AdcTreeChannelCalibration::AdcTreeChannelCalibration(AdcChannelId aid, AdcTime atime)
: m_data(aid.chip, aid.chan, atime) { }

//**********************************************************************

AdcTreeChannelCalibration::
AdcTreeChannelCalibration(AdcChannelId aid, AdcTime atime,
                          Float again, Float aoffset,
                          const FloatVector& acalMeans,
                          const FloatVector& acalRmss,
                          const ShortIndexVector& acalCounts)
: m_data(aid.chip, aid.chan, atime, again, aoffset, acalMeans, acalRmss, acalCounts) { }

//**********************************************************************

AdcTreeChannelCalibration::
AdcTreeChannelCalibration(const AdcTreeChannelCalibrationData& data)
: m_data(data) { }

//**********************************************************************

AdcTreeChannelCalibration::
AdcTreeChannelCalibration(const AdcTreeChannelCalibration& cal)
: AdcTreeChannelCalibration(cal.data()) {
  m_name = cal.name();
}

//**********************************************************************

AdcTreeChannelCalibration& AdcTreeChannelCalibration::
operator=(const AdcTreeChannelCalibration& cal) {
  if ( &cal != this ) {
    m_data = cal.data();
    m_name = cal.name();
  }
  return *this;
}

//**********************************************************************
// Other class methods.
//**********************************************************************

void AdcTreeChannelCalibration::setName(Name cname) {
  m_name = cname;
}

//**********************************************************************

string AdcTreeChannelCalibration::name() const {
  return m_name;
}

//**********************************************************************

Index AdcTreeChannelCalibration::chip() const {
  return m_data.chip;
}

//**********************************************************************

Index AdcTreeChannelCalibration::channel() const {
  return m_data.chan;
}

//**********************************************************************

Index AdcTreeChannelCalibration::time() const {
  return m_data.time;
}

//**********************************************************************

Index AdcTreeChannelCalibration::size() const {
  return m_data.calMeans.size();
}

//**********************************************************************

double AdcTreeChannelCalibration::calMean(AdcCode code) const {
  if ( code >= m_data.calMeans.size() ) return -999.0;
  return m_data.calMeans[code];
}

//**********************************************************************

double AdcTreeChannelCalibration::calRms(AdcCode code) const {
  if ( code >= m_data.calRmss.size() ) return -1.0;
  return m_data.calRmss[code];
}

//**********************************************************************

double AdcTreeChannelCalibration::calTail(AdcCode code) const {
  if ( code >= m_data.calTails.size() ) return -999.0;
  return m_data.calTails[code];
}

//**********************************************************************

ShortIndex AdcTreeChannelCalibration::calCount(AdcCode code) const {
  if ( code >= m_data.calCounts.size() ) return 0;
  return m_data.calCounts[code];
}

//**********************************************************************

double AdcTreeChannelCalibration::linearOffset() const {
  return m_data.offset;
}

//**********************************************************************

double AdcTreeChannelCalibration::linearGain() const {
  return m_data.gain;
}

//**********************************************************************
