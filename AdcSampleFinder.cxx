// AdcSampleFinder.cxx

#include "AdcSampleFinder.h"
#include "AdcTestSampleReader.h"
#include "AdcBinarySampleReader.h"
#include "AdcFembTreeSampleReader.h"
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
// Local definitions.
//**********************************************************************

namespace {

// Function that finds extrema with two extreama finders and returns the
// results of one if they are consistent.
int findExtrema(const AdcSampleReader* prdr, AdcExtrema& exts,
                AdcExtremaFinder& finder1,
                AdcBinExtremaFinder& finder2) {
  const string myname = "AdcSampleFinder::findExtrema: " ;
  AdcExtrema exts1;
  int rstat = finder1.find(*prdr, exts1);
  if ( rstat ) cout << myname << "Extrema 1 search returned error " << rstat << endl;
  AdcExtrema exts2;
  rstat = finder2.find(*prdr, exts2);
  if ( rstat ) cout << myname << "Extrema 2 search returned error " << rstat << endl;
  // Display extrema.
  if ( true ) {
    cout << myname << "Extrema 1:" << endl;
    for ( AdcExtremum ext : exts1 ) {
      cout << myname << setw(12) << ext.tick() << " " << ext.isMax() << endl;
    }
    cout << myname << "Extrema 2:" << endl;
    for ( AdcExtremum ext : exts2 ) {
      cout << myname << setw(12) << ext.tick() << " " << ext.isMax() << endl;
    }
  }
  // Check extrema are consistent.
  Index next = 0;
  if ( exts1.size() == exts2.size() ) next = exts1.size();
  SampleIndex maxdext = 10000;
  for ( Index iext=0; iext<next; ++iext ) {
    AdcExtremum ext1 = exts1[iext];
    AdcExtremum ext2 = exts2[iext];
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
    return 1;
  }
  exts = exts2;
  return 0;
}

}  // end unnamed namespace

//**********************************************************************

Name AdcSampleFinder::defaultTopdir(Name setValue) {
  const string myname = "AdcSampleFinder::topdir: ";
  static string mytopdirRaw = "$HOME/data/dune/adc";
  static string mytopdir;
  static bool firstCall = true;
  if ( setValue.size() ) {
    if ( firstCall ) mytopdirRaw = setValue;
    else if ( setValue != mytopdirRaw && setValue != mytopdir ) {
      cout << myname << " WARNING: New value ignored: " << setValue << endl;
    }
  }
  if ( firstCall ) {
    mytopdir = gSystem->ExpandPathName(mytopdirRaw.c_str());
  }
  firstCall = false;
  return mytopdir;
}

//**********************************************************************

AdcSampleFinder::AdcSampleFinder(Name a_topdir) {
  const string myname = "AdcSampleFinder::ctor: ";
  if ( a_topdir.size() ) {
    m_topdir = gSystem->ExpandPathName(a_topdir.c_str());
  } else {
    m_topdir = defaultTopdir();
  }
}

//**********************************************************************

AdcSampleReaderPtr AdcSampleFinder::find(Name ssam, Index icha, SampleIndex maxsam) const {
  const string myname = "AdcSampleFinder::find: ";
  // CSV samples from Hucheng et al.
  if ( ssam.substr(0,4) == "2016" ||
       ssam.substr(0,6) == "201701" ||
       ssam.substr(0,6) == "201702" ||
       ssam.substr(0,8) == "20170306" ||
       ssam.substr(0,8) == "20170307" ||
       ssam.substr(0,7) == "201703a" ) {
    return AdcSampleReaderPtr(new AdcTestSampleReader(ssam, icha, maxsam));
  }
  // Binary samples from Hucheng et al.
  if ( ssam.substr(0,7) == "201703b" ) {
    SampleIndex maxsam201703b = 39600000;  // Must cut off the data to avoid problem at end.
    if ( maxsam == 0 || maxsam > maxsam201703b ) maxsam = maxsam201703b;
    return findBinaryReader(ssam, icha, maxsam);
  }
  // DUNE test data summer 2017
  if ( ssam.substr(0,7) == "DUNE17-" ) {
    return findFembReader(ssam, icha, maxsam);
  }
  cout << myname << "ERROR: Unable to find reader for sample " << ssam << endl;
  return nullptr;
}

//**********************************************************************

AdcSampleReaderPtr AdcSampleFinder::
findBinaryReader(Name ssam, Index icha, SampleIndex maxsam) const {
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
  // Read the data.
  AdcBinarySampleReader* prdrFull = new AdcBinarySampleReader(fname, ssam, ichp, schpLabel, icha, fsamp, itime, maxsam);
  AdcSampleReaderPtr prdr(prdrFull);
  // Find extrema.
  AdcBorderExtremaFinder ef1(5000000, 50, 4095, 500, 3500);
  AdcBinExtremaFinder ef2(50000, 500, 500);
  AdcExtrema exts;
  if ( findExtrema(&*prdr, exts, ef1, ef2) ) {
    cout << myname << "Unable to find extrema." << endl;
  } else {
    SampleFunction* pfun = new Sawtooth(-300, 1700, exts);
    prdrFull->setSampleFunction(pfun);
  }
  // Build ADC-voltage table.
  prdr->buildTableFromWaveform(20000, 0.1, -300.0);
  return prdr;
}

//**********************************************************************

AdcSampleReaderPtr AdcSampleFinder::
findFembReader(Name asample, Index icha, SampleIndex maxsam) const {
  const string myname = "AdcSampleFinder::findFembReader: ";
  // Check channel.
  if ( icha > 15 ) {
    cout << myname << "Invalid channel: " << icha << endl;
    return nullptr;
  }
  string ssam = asample;
  AdcFembTreeSampleReader* prdrFemb = nullptr;
  string fname;
  string dir;
  string::size_type ipos = asample.find("_");
  string dsname = asample.substr(0, ipos);
  vector<string> sels;
  sels.push_back("adcTestData");
  sels.push_back("functype3");
  sels.push_back("sampleRate2000000");
  if ( asample.substr(0,14) == "DUNE17-test2" ) {
    string basename = "adcTestData_20170613T172751_chip26_adcClock1_adcOffset-1_sampleRate2000000_functype3_freq734.000_offset0.700_amplitude1.000_calib.root";
    fname = AdcSampleFinder::defaultTopdir() + "/justin2/" + basename;
    ipos = 14;
    dsname += "-test2";
  } else if ( asample.substr(0,14) == "DUNE17-test3" ) {
    string basename = "adcTestData_20170622T183049_chip0_functype3_freq4.000_offset0.700_amplitude1.000.root";
    fname = AdcSampleFinder::defaultTopdir() + "/justin3/" + basename;
    ipos = 14;
    dsname += "-test2";
  } else if ( dsname == "DUNE17-cold" ) {
    dir = AdcSampleFinder::defaultTopdir() + "/DUNE17/adcTest_P1single_cold/";
    if ( asample.substr(ipos, 5) != "_chip" ) {
      cout << myname << "Chip ID not found." << endl;
      return nullptr;
    }
    string::size_type jpos = asample.find("_", ipos+5);
    ++ipos;
    string sfieldChip = asample.substr(ipos, jpos-ipos);
    sels.push_back(sfieldChip);
  } else {
    cout << myname << "Unable to find FEMB sample " << ssam << endl;
  }
  // Find the file.
  if ( fname.size() == 0 ) {
    FileDirectory filedir(dir);
    for ( string sel : sels ) filedir.select(sel);
    unsigned int nfile = filedir.files.size();
    if ( nfile == 1 ) {
      fname = filedir.dirname + "/" + filedir.files.begin()->first;
    } else if ( nfile == 0 ) {
      cout << myname << "ERROR: Unable to find file with the following patterns:" << endl;
      for ( string sel : sels ) cout << myname << "  " << sel << endl;
      cout << myname << "Searched directory " << filedir.dirname << endl;
      return nullptr;
    } else {
      cout << myname << "ERROR: Found multiple files:" << endl;
      for ( const auto& file : filedir.files ) cout << myname << "  " << file.first << endl;
      cout << myname << "Searched directory " << filedir.dirname << endl;
      return nullptr;
    }
  }
  // Find the sample range.
  SampleIndex isam0 = 0;
  SampleIndex nsam = 0;
  if ( ssam.size() > ipos ) {
    ipos = ssam.find("_", ipos);
    string::size_type jpos = ssam.find("_", ipos+1);
    if ( jpos != string::npos ) {
      istringstream ssisam0(ssam.substr(ipos+1, jpos-ipos-1));
      ssisam0 >> isam0;
      istringstream ssnsam(ssam.substr(jpos+1));
      ssnsam >> nsam;
    }
  }
  prdrFemb = new AdcFembTreeSampleReader(fname, icha, ssam, isam0, nsam);
  AdcSampleReaderPtr prdr(prdrFemb);
  // Find extrema.
  bool calculateVin = true;
  if ( calculateVin ) {
    double vinRate = prdrFemb->vinFreq();
    if ( vinRate <= 0.0 ) {
      vinRate = 4;
      cout << myname << "Generator frequency not found. Assuming " << vinRate << " Hz." << endl;
    }
    double sampFreq = prdrFemb->samplingFrequency();
    if ( sampFreq <= 0.0 ) {
      sampFreq = 2000000;
      cout << myname << "Sampling frequency not found. Assuming " << sampFreq << " Hz." << endl;
    }
    double tickPeriod = sampFreq/vinRate;
    double borderWidth = 0.2*tickPeriod;
    //AdcBorderExtremaFinder ef(borderWidth, 500, 4000, 1500, 3800);
    AdcBorderExtremaFinder ef(borderWidth, 500, 4000, 1500,    0);
    AdcExtrema exts;
    int rstat = ef.find(*prdr, exts);
    if ( rstat ) {
      cout << myname << "Extrema finding failed with error " << rstat << "." << endl;
    } else if ( exts.size() == 0 ) {
      cout << myname << "No extrema found." << endl;
    } else {
      cout << myname << "Extrema:" << endl;
      for ( Index iext=0; iext<exts.size(); ++iext ) {
        AdcExtremum ext = exts[iext];
        cout << myname << setw(12) << ext.tick() << " " << ext.isMax();
        if ( iext ) cout << setw(12) << ext.tick() - exts[iext-1].tick();
        cout << endl;
      }
      SampleFunction* pfun = new Sawtooth(-300, 1700, exts);
      prdrFemb->setSampleFunction(pfun);
    }
  }
  // Build ADC-voltage table.
  cout << myname << "Building ADC-Vin table." << endl;
  prdr->buildTableFromWaveform(20000, 0.1, -300.0);
  cout << myname << "Done building ADC-Vin table." << endl;
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
