// AdcTreeChannelCalibration.cxx

#include "AdcTreeChannelCalibration.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>

using std::cout;
using std::endl;
using Name = std::string;
using CalibMap = std::map<AdcChannelId, const AdcTreeChannelCalibration*>;
using CalibMapMap = std::map<Name, CalibMap>;

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
    cout << myname << "Unable to find file " << fname << endl;
    return nullptr;
  }
  TTree* ptree = dynamic_cast<TTree*>(pfile->Get("adccalib"));
  if ( ptree==nullptr ) {
    cout << myname << "Unable to find tree adccalib in file " << fname << endl;
    return nullptr;
  }
  const AdcTreeChannelCalibration* pcal = new AdcTreeChannelCalibration;
  ptree->SetBranchAddress("cal", &pcal);
  for ( unsigned int ient=0; ient<ptree->GetEntries(); ++ient ) {
    ptree->GetEntry(ient);
    if ( pcal->chip == aid.chip && pcal->chan == aid.chan ) {
      pcalout = new AdcTreeChannelCalibration(*pcal);
      break;
    }
  }
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

AdcTreeChannelCalibration::AdcTreeChannelCalibration()
: time(0), gain(0.0), offset(0.0) { }

//**********************************************************************

AdcTreeChannelCalibration::AdcTreeChannelCalibration(AdcChannelId aid, AdcTime atime)
: chip(aid.chip), chan(aid.chan), time(atime), gain(0.0), offset(0.0) { }

//**********************************************************************

AdcTreeChannelCalibration::
AdcTreeChannelCalibration(AdcChannelId aid, AdcTime atime,
                          Float again, Float aoffset,
                          const FloatVector& acalMeans,
                          const FloatVector& acalRmss,
                          const ShortIndexVector& acalCounts)
: chip(aid.chip), chan(aid.chan), time(atime), gain(again), offset(aoffset),
  calMeans(acalMeans), calRmss(acalRmss), calCounts(acalCounts) { }

//**********************************************************************

Float AdcTreeChannelCalibration::calMean(AdcCode code) const {
  if ( code >= calMeans.size() ) return -999.0;
  return calMeans[code];
}

//**********************************************************************

Float AdcTreeChannelCalibration::calRms(AdcCode code) const {
  if ( code >= calRmss.size() ) return -1.0;
  return calRmss[code];
}

//**********************************************************************

Float AdcTreeChannelCalibration::calTail(AdcCode code) const {
  if ( code >= calTails.size() ) return -999.0;
  return calTails[code];
}

//**********************************************************************

ShortIndex AdcTreeChannelCalibration::calCount(AdcCode code) const {
  if ( code >= calCounts.size() ) return 0;
  return calCounts[code];
}

//**********************************************************************
