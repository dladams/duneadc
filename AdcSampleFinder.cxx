// AdcSampleFinder.cxx

#include "AdcSampleFinder.h"
#include "AdcTestSampleReader.h"
#include "AdcBinarySampleReader.h"
#include "AdcBorderExtremaFinder.h"
#include "AdcBinExtremaFinder.h"
#include "Sawtooth.h"
#include "FileDirectory.h"
#include "TSystem.h"
#include "TDatime.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

using std::string;
using std::cout;
using std::endl;
using std::istringstream;
using std::setw;

using Name = std::string;
using NameVector = std::vector<Name>;
using AdcSampleReaderPtr = AdcSampleFinder::AdcSampleReaderPtr;

//**********************************************************************

AdcSampleFinder::AdcSampleFinder(Name topdir) {
  const string myname = "AdcSampleFinder::ctor: ";
  const string deftopdir = "$HOME/data/dune/adc";
  m_topdir = gSystem->ExpandPathName(deftopdir.c_str());
  if ( topdir != deftopdir ) {
    cout << myname << " WARNING: Location must be set to " << deftopdir << endl;
  }
}

//**********************************************************************

AdcSampleReaderPtr AdcSampleFinder::find(Name ssam, Index icha, Index maxsam) const {
  const string myname = "AdcSampleFinder::find: ";
  // CSV samples from Hucheng et al.
  if ( ssam.substr(0,4) == "2016" ||
       ssam.substr(0,6) == "201702" ||
       ssam.substr(0,8) == "20170306" ||
       ssam.substr(0,8) == "20170307" ||
       ssam.substr(0,7) == "201703a" ) {
    return AdcSampleReaderPtr(new AdcTestSampleReader(ssam, icha, maxsam));
  }
  // Binary samples from Hucheng et al.
  if ( ssam.substr(0,7) == "201703b" ) {
    return findBinaryReader(ssam, icha, maxsam);
  }
  cout << myname << "ERROR: Unable to find reader for sample " << ssam << endl;
  return nullptr;
}

//**********************************************************************

AdcSampleReaderPtr AdcSampleFinder::
findBinaryReader(Name ssam, Index icha, Index maxsam) const {
  const string myname = "AdcSampleFinder::findBinaryReader: ";
  // Long term tests: 201703b_CCC_DDD where CCC=chip=D02 and DDD = day
  string dsname;         // Dataset name.
  string schp;           // Chip index within the dataset.
  string schpLabel;      // Global chip label.
  string scha;           // Channel number
  string dir;            // Directory where file resides.
  string spat;           // String pattern that specifies the file.
  double fsamp = 2.0e6;  // Sampling frequency [Hz]
  if ( ssam.substr(0, 7) == "201703b" ) {
    if ( ssam.size() != 14 ||
         ssam.substr(7,2) != "_D" ||
         ssam.substr(11,1) != "_" ) {
      cout << myname << "ERROR: Invalid 201703b sample name: " << ssam << endl;
      return nullptr;
    }
    dsname = ssam.substr(0, 7);
    schpLabel = ssam.substr(8,3);
    schp = ssam.substr(9,2);
    string sday = ssam.substr(12,3);
    scha = schan(icha);
    if ( scha.size() == 0 ) {
      cout << myname << "ERROR: Invalid channel: " << icha << endl;
      return nullptr;
    }
    string subdir = "P1_S7_" + schp + "_" + sday;
    dir = m_topdir + "/201703/P1_ADC_LongTermTest_03212017/" + subdir;
    spat = subdir + "_LN_2MHz_chn" + scha;
  }
  // Find the file.
  FileDirectory filedir(dir);
  FileDirectory::FileMap files = filedir.find(spat);
  Name fname;
  if ( files.size() == 1 ) {
    fname = filedir.dirname + "/" + files.begin()->first;
  } else {
    cout << myname << "ERROR: Unable to find file with pattern " << spat
         << " in directory " << filedir.dirname << endl;
    return nullptr;
  }
  istringstream sschp(schp);
  Index ichp = badChip();
  sschp >> ichp;
  istringstream sscha(scha);
  // Find the time.
  string::size_type ipos;
  ipos = fname.find("_chn");
  ipos = fname.find("_", ipos+1);
  ++ipos;
  int month  = std::atoi(fname.substr(ipos+0, 2).c_str());
  int day    = std::atoi(fname.substr(ipos+3, 2).c_str());
  int hour   = std::atoi(fname.substr(ipos+6, 2).c_str());
  int minute = std::atoi(fname.substr(ipos+9, 2).c_str());
  TDatime datime(2017, month, day, hour, minute, 0);
  AdcTime itime = datime.Convert();
  AdcBinarySampleReader* prdrFull = new AdcBinarySampleReader(fname, ssam, ichp, schpLabel, icha, fsamp, itime);
  AdcSampleReaderPtr prdr(prdrFull);
  // Find extrema.
  AdcExtrema borderExts;
  {
    AdcBorderExtremaFinder ef(4000000, 50, 4095, 500, 3500);
    ef.find(*prdr, borderExts);
  }
  AdcExtrema binExts;
  {
    AdcBinExtremaFinder ef(50000, 500, 500);
    ef.find(*prdr, binExts);
  }
  // Display extrema.
  if ( true ) {
    cout << myname << "Border extrema:" << endl;
    for ( AdcExtremum ext : borderExts ) {
      cout << myname << setw(12) << ext.tick() << " " << ext.isMax() << endl;
    }
    cout << myname << "Bin extrema:" << endl;
    for ( AdcExtremum ext : binExts ) {
      cout << myname << setw(12) << ext.tick() << " " << ext.isMax() << endl;
    }
  }
  // Check extrema are consistent.
  Index next = 0;
  if ( borderExts.size() == binExts.size() ) next = borderExts.size();
  SampleIndex maxdext = 10000;
  for ( Index iext=0; iext<next; ++iext ) {
    AdcExtremum ext1 = borderExts[iext];
    AdcExtremum ext2 = binExts[iext];
    if ( ext1.isMin() != ext2.isMin() ) next = 0;
    else {
      SampleIndex dext = ext2.tick() > ext1.tick() ?
                         ext2.tick() - ext1.tick() :
                         ext1.tick() - ext2.tick();
      if ( dext > maxdext ) next = 0;
    }
  }
  if ( next == 0 ) {
    cout << myname << "ERROR: No extrema found." << endl;
  } else {
    SampleFunction* pfun = new Sawtooth(-300, 1700, binExts);
    prdrFull->setSampleFunction(pfun);
  }
  // Build ADC-voltage table.
  prdr->buildTableFromWaveform(20000, 0.1, -300.0);
  return prdr;
}

//**********************************************************************

Name AdcSampleFinder::schan(Index icha) const {
  static NameVector schans = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                              "A", "B", "C", "D", "E", "F"};
  if ( icha > schans.size() ) return "";
  return schans[icha];
}

//**********************************************************************
