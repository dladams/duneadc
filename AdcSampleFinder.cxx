// AdcSampleFinder.cxx

#include "AdcSampleFinder.h"
#include "AdcTestSampleReader.h"
#include "AdcBinarySampleReader.h"
#include "AdcFembTreeSampleReader.h"
#include "AdcBorderExtremaFinder.h"
#include "AdcBinExtremaFinder.h"
#include "RollbackFinder.h"
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
                AdcBinExtremaFinder& finder2,
                SampleIndex maxdext) {
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
  Index d12 = 0;
  if ( exts1.size() == exts2.size() ) {
    next = exts1.size();
  } else if ( exts1.size() + 1 == exts2.size() ) {
    if ( exts1[0].isMin() != exts2[0].isMin() ) {
      cout << myname << "Border search did not find the first extremum." << endl;
      d12 = 1;
    } else {
      cout << myname << "Border search did not find the last extremum." << endl;
    }
    next = exts1.size();
  } else if ( exts1.size() + 2 == exts2.size() ) {
    cout << myname << "Border search did not find the first and last extrema." << endl;
    d12 = 1;
    next = exts1.size();
  } else {
    cout << myname << "ERROR: Inconsistent extrema counts." << endl;
  }
  for ( Index iext1=0; iext1<next; ++iext1 ) {
    Index iext2 = iext1 + d12;
    AdcExtremum ext1 = exts1[iext1];
    AdcExtremum ext2 = exts2[iext2];
    if ( ext1.isMin() != ext2.isMin() ) next = 0;
    else {
      SampleIndex dext = ext2.tick() > ext1.tick() ?
                         ext2.tick() - ext1.tick() :
                         ext1.tick() - ext2.tick();
      if ( dext > maxdext ) {
        cout << myname << "ERROR: Too large dext: " << dext << " > " << maxdext << endl;
        next = 0;
      } else if ( dext > 10000 ) {
        cout << myname << "WARNING: Allowing large dext: " << dext << " < " << maxdext << endl;
      }
    }
  }
  if ( next == 0 ) {
    cout << myname << "ERROR: No extrema found." << endl;
    return 1;
  }
  // Use results from bin extrema finder.
  // Border finder is biased because it starts search from smaller ticks.
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

bool AdcSampleFinder::isBadDune17(Index chip, Index chan, AdcTime time) {
  bool anyChannel = chan == badChannel();
  if ( chan > 15 && !anyChannel ) return true;
/*  This is now handled in dune17.py
  if ( chip ==  1 ) {
    if ( time == 1498867440 ) return true;   // square wf
  } else if ( chip == 19 ) {
    if ( time == 1499436420 ) return true;   // empty wf
  } else if ( chip == 24 ) {
    if ( time == 1499465100 ) return true;   // empty wf
  } else if ( chip == 25 ) {
    if ( time == 1499387520 ) return true;   // empty wf
  } else if ( chip == 27 ) {
    // Waveform looks ok for chan 0-4 but does not quite reach saturation.
    if ( time == 1498657500 ) return true;
  } else if ( chip == 28 ) {
    if ( time == 1498854600 ) return true;
  } else if ( chip == 62 ) {
    if ( time == 1498668840 ) return true;   // wf is very ugly
  } else if ( chip == 85 ) {
    if ( time == 1498828620 ) return true;   // wf empty
  } else if ( chip == 88 ) {
    if ( time == 1498835580 ) return true;   // wf mostly empty
  } else if ( chip == 89 ) {
    if ( time == 1498837620 ) return true;   // square wf
  } else if ( chip == 109 ) {
    if ( time == 1499781660 ) return true;   // bad wf
  } else if ( chip == 114 ) {
    if ( time == 1499781900 ) return true;   // bad wf
  } else if ( chip == 116 ) {
    if ( time == 1499708640 ) return true;   // bad wf
  } else if ( chip == 117 ) {
    if ( time == 1499713680 ) return true;   // bad wf
  } else if ( chip == 123 ) {
    if ( time == 1499727120 ) return true;   // bad wf
  } else if ( chip == 126 ) {
    if ( time == 1499803680 ) return true;   // bad wf
  } else if ( chip == 128 ) {
    if ( time == 1499734440 ) return true;   // bad wf
  }
*/
  return false;
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
  AdcSampleReaderPtr prdr;
  if ( ssam.substr(0,4) == "2016" ||
       ssam.substr(0,6) == "201701" ||
       ssam.substr(0,6) == "201702" ||
       ssam.substr(0,8) == "20170306" ||
       ssam.substr(0,8) == "20170307" ||
       ssam.substr(0,7) == "201703a" ||
       ssam.substr(0,11) == "201706_cots" ||
       ssam.substr(0,11) == "201707_cots" ) {
    return AdcSampleReaderPtr(new AdcTestSampleReader(ssam, icha, maxsam));
  }
  // Binary samples from Hucheng et al.
  if ( ssam.substr(0,7) == "201703b" ) {
    SampleIndex maxsam201703b = 39600000;  // Must cut off the data to avoid problem at end.
    if ( maxsam == 0 || maxsam > maxsam201703b ) maxsam = maxsam201703b;
    return findBinaryReader(ssam, icha, maxsam);
  }
  if ( ssam.substr(0,9) == "DUNE17ts-" ) {
    SampleIndex maxsamHere = 19800000;  // Must cut off the data to avoid problem at end.
    if ( maxsam == 0 || maxsam > maxsamHere ) maxsam = maxsamHere;
    return findBinaryReader(ssam, icha, maxsam);
  }
  // DUNE test data analyzed same as March data.
  if ( ssam.substr(0,11) == "DUNE17-test" ) {
    SampleIndex maxsamHere = 19800000UL;  // Must cut off the data to avoid problem at end.
    if ( maxsam == 0 || maxsam > maxsamHere ) maxsam = maxsamHere;
    return findBinaryReader(ssam, icha, maxsam);
  }
  // DUNE test data summer 2017
  if ( ssam.substr(0,7) == "DUNE17-" ) {
    prdr = findFembReader(ssam, icha, maxsam);
  }
  if ( ! prdr ) {
    cout << myname << "ERROR: Unable to find reader for sample " << ssam << endl;
  }
  return prdr;
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
  vector<string> searchDirs;   // Directories to search for data files.
  vector<string> searchPats;   // File patterns in those directories.
  double fsamp = 2.0e6;  // Sampling frequency [Hz]
  SampleIndex ef1BorderWidth = 5000000;
  SampleValue ef1MinThresh =   50;
  SampleValue ef1MaxThresh = 4095;
  SampleValue ef1MinLimit =   500;
  SampleValue ef1MaxLimit =  3500;
  Index ef2NbinThresh = 500;
  Index ef2MinGapBin = 50000;
  SampleIndex maxdext = 10000;
  double vinMin = -300.0;
  double vinMax = 1700.0;
  Index maxRollback = 0; // If nonzero, rollback is removed for up to this many samples.
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
    searchDirs.push_back(m_topdir + "/201703/P1_ADC_LongTermTest_03212017/" + subdir);
    searchPats.push_back(subdir + "_LN_2MHz_chn" + scha);
  } else if ( ssam.substr(0,11) == "DUNE17-test" ) {
    string::size_type ipos = 11;
    dsname = ssam.substr(0, ipos);
    if ( ssam.substr(ipos, 5) != "_chip" ) {
      cout << myname << "Chip ID not found." << endl;
      return nullptr;
    }
    ipos += 5;
    string::size_type jpos = ssam.find("_", ipos);
    schp = ssam.substr(ipos, jpos-ipos);
    scha = schan(icha);
    searchDirs.push_back(m_topdir + "/DUNE17t/P1_ADC_Test_Data_06262017/P1_ADC_Test_Data_0615/P1_S7_0" + schp);
    searchPats.push_back("P1_S7_0" + schp + "_LN_2MHz_chn" + scha);
    ef1BorderWidth = 1500000;
    ef1MinThresh =  700;
    ef1MaxThresh = 4000;
    ef1MinLimit  = 2000;
    ef1MaxLimit  =    0;
    //ef2MinGapBin = 5000000;
    ef2NbinThresh = 200;
    maxdext = 100000;
    vinMin = -200.0;
    vinMax = 1800.0;
  } else if ( ssam.substr(0,14) == "DUNE17ts-cold_" ) {
    string::size_type ipos = 14;
    dsname = ssam.substr(0, ipos);
    if ( ssam.substr(ipos, 4) != "chip" ) {
      cout << myname << "Chip ID not found." << endl;
      return nullptr;
    }
    ipos += 4;
    string::size_type jpos = ssam.find("_", ipos);
    string::size_type npos = jpos;
    string suf;  // If there is a suffix, only dirs containing it are searched
    string schpFile = schp;
    if ( npos != string::npos ) {
      npos = jpos - ipos;
      suf = ssam.substr(jpos+1);
      cout << "Suffix: " << suf << endl;
      // If suffix starts with fchipCCC then CCC is used as the chip in the file pattern. 
      if ( suf.substr(0,5) == "fchip" ) {
        schp = ssam.substr(ipos, npos);
        ipos += 5;
        jpos = ssam.find("_", ipos);
        npos = jpos;
        if ( npos != string::npos ) {
          npos = jpos - ipos;
          suf = ssam.substr(jpos+1);
        } else {
          suf = "";
        }
      } 
    }
    schpFile = ssam.substr(ipos, npos);
    if ( schp == "" ) schp = schpFile;
    scha = schan(icha);
    vector<string> subdirs = {
      "P1_ADC_07172017",
      "P1_ADC_0718",
      "P1_ADC_0719",
      "P1_ADC_0720",
      "P1_ADC_0721",
      "P1_ADC_0724",
      "P1_ADC_0725",
      "P1_ADC_0726",
      "P1_ADC_0727",
      "P1_ADC_0727_DNL_error"
    };
    string dirpat = "P1_S7_" + schpFile + "_";
    string filpat = dirpat;
    // Special values for 7/26 DNL data.
    string fileDate;    // Enough of file date pattern to distinuish samples, eg 07_25_16
    if ( suf.substr(0,3) == "DNL" ) {
      subdirs.clear();
      subdirs.push_back("P1_ADC_0726_DNL_error");
      //dirpat = "DNL error P1_SS7_" + schpFile + "_";
      dirpat = "error P1_SS7_" + schpFile + "_";
      filpat = "P1_S7_" + schpFile + "_";
      if ( suf.size() > 3 ) fileDate = suf.substr(3);
      suf = "";
    }
    for ( string subdir : subdirs ) {
      if ( suf.size() != 0 &&
           subdir.find(suf) == string::npos ) continue;
      string searchDir = m_topdir + "/DUNE17ts/" + subdir;
      FileDirectory fd(searchDir);
      for ( FileDirectory::FileMap::value_type ent : fd.find(dirpat) ) {
        string subsubdir = ent.first;
        string filepre = subsubdir;
        if ( filepre.substr(0,17) == "DNL error P1_SS7_" ||
             filepre.substr(0,17) == "DNL_error P1_SS7_" ) {
          filepre.replace(0, 17, "P1_S7_");
        }
        searchDirs.push_back(searchDir + "/" + subsubdir);
        searchPats.push_back(filepre + "_LN_2MHz_chn" + scha + "_" + fileDate);
      }
    }
    ef1BorderWidth = 1500000;
    ef1MinThresh =  700;
    ef1MaxThresh = 4000;
    ef1MinLimit  = 2000;
    ef1MaxLimit  =    0;
    //ef2MinGapBin = 5000000;
    ef2NbinThresh = 200;
    maxdext = 100000;
    vinMin = -200.0;
    vinMax = 1800.0;
    maxRollback = 2000000;
  }
  if ( schpLabel == "" ) schpLabel = schp;
  // Find the file.
  if ( searchDirs.size() == 0 ) {
    cout << myname << "ERROR: No search directories found for sample " << ssam << endl;
    return nullptr;
  }
  vector<string> foundFiles;
  for ( Index isrc=0; isrc<searchDirs.size(); ++isrc ) {
    string sdir = searchDirs[isrc];
    string spat = searchPats[isrc];
    FileDirectory fd(sdir);
    for ( FileDirectory::FileMap::value_type ent : fd.find(spat) ) {
      foundFiles.push_back(sdir + "/" + ent.first);
    }
  }
  Name fname;
  if ( foundFiles.size() == 1 ) {
    fname = foundFiles.front();
  } else if ( foundFiles.size() == 0 ) {
    cout << myname << "ERROR: Unable to find file." << endl;
    cout << myname << "Search dirs and patterns:" << endl;
    for ( Index isrc=0; isrc<searchDirs.size(); ++isrc ) {
      string sdir = searchDirs[isrc];
      string spat = searchPats[isrc];
      cout << myname << "  " << sdir << ": " << spat  << endl;
    }
    return nullptr;
  } else {
    cout << myname << "ERROR: Multiple file match: " << endl;
    for ( string foundFile : foundFiles ) cout << myname << "  " << foundFile << endl;
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
  // Mitigate rollback.
  if ( maxRollback ) {
    prdr->addMitigator(new RollbackFinder(*prdr, maxRollback));
  }
  // Find extrema.
  AdcBorderExtremaFinder ef1(ef1BorderWidth, ef1MinThresh, ef1MaxThresh, ef1MinLimit, ef1MaxLimit);
  AdcBinExtremaFinder ef2(ef2MinGapBin, 500, ef2NbinThresh);
  AdcExtrema exts;
  if ( findExtrema(&*prdr, exts, ef1, ef2, maxdext) ) {
    cout << myname << "Unable to find extrema." << endl;
  } else {
    SampleFunction* pfun = new Sawtooth(vinMin, vinMax, exts);
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
  vector<string> dirs;
  string::size_type ipos = asample.find("_");
  string dsname = asample.substr(0, ipos);
  vector<string> sels;
  sels.push_back("adcTestData");
  sels.push_back("functype3");
  sels.push_back("sampleRate2000000");
  bool isDune17 = false;
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
    dirs.push_back(AdcSampleFinder::defaultTopdir() + "/DUNE17/adcTest_P1single_cold/");
    dirs.push_back(AdcSampleFinder::defaultTopdir() + "/DUNE17/adcTest_P1single_hothdaq4_cold/");
    dirs.push_back(AdcSampleFinder::defaultTopdir() + "/DUNE17/adcTest_P1single_hothdaq5_cold/");
    if ( asample.substr(ipos, 5) != "_chip" ) {
      cout << myname << "Chip ID not found." << endl;
      return nullptr;
    }
    string::size_type jpos = asample.find("_", ipos+5);
    ++ipos;
    string::size_type len = jpos == string::npos ? string::npos : jpos-ipos;
    string sfieldChip = asample.substr(ipos, len);
    sels.push_back(sfieldChip + "_");
    isDune17 = true;
    if ( jpos != string::npos ) sels.push_back(asample.substr(jpos+1));
  } else if ( dsname == "DUNE17-longramp" ) {
    dirs.push_back(AdcSampleFinder::defaultTopdir() + "/DUNE17/longramp/");
    if ( asample.substr(ipos, 5) != "_chip" ) {
      cout << myname << "Chip ID not found." << endl;
      return nullptr;
    }
    cout << myname << "Need more code here..." << endl;
    return nullptr;
  } else {
    cout << myname << "Unable to find FEMB sample " << ssam << endl;
  }
  // Find the file.
  if ( fname.size() == 0 ) {
    map<string, string> files;
    for ( string dir : dirs ) {
      FileDirectory filedir(dir);
      for ( string sel : sels ) filedir.select(sel);
      for ( auto ent : filedir.files ) {
        string fname = ent.first;
        if ( files.find(fname) != files.end() ) {
          cout << myname << "WARNING: Skipping duplicate file entry." << endl;
        } else {
          string fullname = filedir.dirname + "/" + fname;
          files[fname] = fullname;
        }
      }
    }
    unsigned int nfile = files.size();
    if ( nfile == 1 ) {
      fname = files.begin()->second;
    } else if ( nfile == 0 ) {
      cout << myname << "ERROR: Unable to find file with the following patterns:" << endl;
      for ( string sel : sels ) cout << myname << "  " << sel << endl;
      cout << myname << "Search directories:" << endl;
      for ( string dir : dirs ) cout << myname << "  " << dir << endl;
      cout << endl;
      return nullptr;
    } else {
      cout << myname << "ERROR: Found multiple files:" << endl;
      for ( const auto& filepath : files ) cout << myname << "  " << filepath.second << endl;
      return nullptr;
    }
  }
  cout << myname << "Sample file name: " << fname << endl;
  // Find the sample range.
  SampleIndex isam0 = 0;
  SampleIndex nsam = 0;
  // Drop the option for samples because the suffix is now used to select between multiple versions.
  if ( false && ssam.size() > ipos ) {
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
  if ( isDune17 ) {
    if ( isBadDune17(prdr->chip(), prdr->channel(), prdr->time()) ) {
      cout << myname << "DUNE17 sample " << prdr->sample() << " channel " << icha
           << " is flagged bad." << endl;
      prdr.reset(nullptr);
      return prdr;
    }
  }
  // Mitigate rollback.
  prdr->addMitigator(new RollbackFinder(*prdr, 100000));
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
    double ef1BorderWidth = 0.2*tickPeriod;
    SampleValue ef1MinThresh =  1000;
    SampleValue ef1MaxThresh =  4000;
    SampleValue ef1MinLimit =   1500;
    SampleValue ef1MaxLimit =      0;
    double vinMin = -300.0;
    double vinMax = 1700.0;
    if ( 1 ) {
      // Find extrema.
      AdcBorderExtremaFinder ef1(ef1BorderWidth, ef1MinThresh, ef1MaxThresh, ef1MinLimit, ef1MaxLimit);
      Index ef2NbinThresh = 500;
      Index ef2MinGapBin = 50000;
      SampleIndex maxdext = 10000;
      AdcBinExtremaFinder ef2(ef2MinGapBin, 500, ef2NbinThresh);
      AdcExtrema exts;
      if ( findExtrema(&*prdr, exts, ef1, ef2, maxdext) ) {
        cout << myname << "Unable to find extrema." << endl;
      } else {
        SampleFunction* pfun = new Sawtooth(vinMin, vinMax, exts);
        prdrFemb->setSampleFunction(pfun);
      }
    } else {
      //AdcBorderExtremaFinder ef(borderWidth, 500, 4000, 1500, 3800);
      //AdcBorderExtremaFinder ef(borderWidth, 500, 4000, 1500,    0);
      // Setting maxLimit = 0 helps with large fluctuations after overflow.
      // Large minThresh fixes t0 offset due to fluctuations near that value.
      AdcBorderExtremaFinder ef(ef1BorderWidth, ef1MinThresh, ef1MaxThresh, ef1MinLimit, ef1MaxLimit);
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
        SampleFunction* pfun = new Sawtooth(vinMin, vinMax, exts);
        prdrFemb->setSampleFunction(pfun);
      }
    }
  }
  // Build ADC-voltage table.
  cout << myname << "Building ADC-Vin table." << endl;
  prdr->buildTableFromWaveform(20000, 0.1, -300.0, true, true);
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
