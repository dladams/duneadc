// AdcChannelCalibration.cxx

#include "AdcChannelCalibration.h"
#include "TFile.h"
#include "TTree.h"
#include <iostream>

using std::cout;
using std::endl;
using ShortIndex = unsigned short;
using Name = std::string;
using Float = AdcChannelCalibration::Float;
using CalibMap = std::map<AdcChannelId, const AdcChannelCalibration*>;
using CalibMapMap = std::map<Name, CalibMap>;

//**********************************************************************

const AdcChannelCalibration*
AdcChannelCalibration::find(std::string dataset, AdcChannelId aid) {
  static CalibMapMap calibmaps;
  const Name myname = "AdcChannelCalibration::find: ";
  CalibMap& calibmap = calibmaps[dataset];
  CalibMap::iterator icm = calibmap.find(aid);
  if ( icm != calibmap.end() ) return icm->second;
  const AdcChannelCalibration*& pcalout = calibmap[aid];
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
  const AdcChannelCalibration* pcal = new AdcChannelCalibration;
  ptree->SetBranchAddress("cal", &pcal);
  for ( unsigned int ient=0; ient<ptree->GetEntries(); ++ient ) {
    ptree->GetEntry(ient);
    if ( pcal->chip == aid.chip && pcal->chan == aid.chan ) {
      pcalout = new AdcChannelCalibration(*pcal);
      break;
    }
  }
  pfile->Close();
  delete pfile;
  return pcalout;
}

//**********************************************************************

const AdcChannelCalibration*
AdcChannelCalibration::find(std::string dataset, ShortIndex chip, ShortIndex chan) {
  return find(dataset, AdcChannelId(chip, chan));
}

//**********************************************************************

AdcChannelCalibration::AdcChannelCalibration()
: gain(0.0), offset(0.0) { }

//**********************************************************************

AdcChannelCalibration::AdcChannelCalibration(AdcChannelId aid)
: chip(aid.chip), chan(aid.chan), gain(0.0), offset(0.0) { }

//**********************************************************************

AdcChannelCalibration::
AdcChannelCalibration(AdcChannelId aid, Float again, Float aoffset,
                      const FloatVector& acalMeans,
                      const FloatVector& acalRmss,
                      const IndexVector& acalCounts)
: chip(aid.chip), chan(aid.chan), gain(again), offset(aoffset),
  calMeans(acalMeans), calRmss(acalRmss), calCounts(acalCounts) { }

//**********************************************************************

Float AdcChannelCalibration::calMean(Index code) const {
  if ( code >= calMeans.size() ) return -999.0;
  return calMeans[code];
}

//**********************************************************************

Float AdcChannelCalibration::calRms(Index code) const {
  if ( code >= calRmss.size() ) return -1.0;
  return calRmss[code];
}

//**********************************************************************

AdcChannelCalibration::Index AdcChannelCalibration::calCount(Index code) const {
  if ( code >= calCounts.size() ) return 0;
  return calCounts[code];
}

//**********************************************************************
