// AdcSampleReader.cxx

#include "AdcSampleReader.h"
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TSystem.h"

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::ostringstream;
using std::getline;

using Name = AdcSampleReader::Name;
using Index = AdcSampleReader::Index;

//**********************************************************************

AdcSampleReader::AdcSampleReader(Name ssam, Index chan, Index maxsam)
: m_topdir(gSystem->ExpandPathName("$HOME/data/dune/adc")),
  m_chip(999),
  m_chan(chan),
  m_nadc(4096),
  m_nvin(0),
  m_dvin(0.1),
  m_vinmin(0.0),
  m_samplingFrequency(2.e6),
  m_dvdt(0.0),
  m_nomVinPerAdc(0.0),
  m_nsample(0) {
  const string myname = "AdcSampleReader::ctor: ";
  string fname;
  string dirname;
  const string aschanold[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};
  const string aschan[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
  string schan = aschanold[chan];
  cout << myname << "Sample " << ssam << " channel " << chan << endl;
  bool isRaw = false;   // True if the input data is a waveform instead of a table
  double vinmax = 0.0;  // If nvin==0 and vinmax>0, then nvin is calculated from vinmax
  int bad = 0;
  string schanPrefix;
  if ( ssam == "201610_vstar" ) {
    fname = m_topdir + "/201610/VSTAR_1S/LN2_vstar_1s_200MHz_vstar_adc_09_28_2016_15_03_35/";
    fname += "LN2_vstar_1s_200MHz_vstar_adc_09_28_2016_15_03_35_chn" + aschanold[chan] + ".csv";
    m_nomVinPerAdc = 1.0/2.346;
  } else if ( ssam == "201610_p1" ) {
    fname = m_topdir + "/201610/P1_ADC_DIE1/LN2_die1_200MHz_p1_adc_09_27_2016_19_18_27/";
    fname += "LN2_die1_200MHz_p1_adc_09_27_2016_19_18_27_chn" + aschanold[chan] + ".csv";
    m_nomVinPerAdc = 1.0/3.033;
  } else if ( ssam == "201611_p1_clkint" ) {
    string smin[16] = {"48", "48", "49", "49", "50", "50", "50", "51",
                       "51", "51", "52", "52", "52", "53", "53", "53"};
    fname = m_topdir + "/201611/P1_die1/intClk_full_LN_2MHz/CSV/";
    fname += "P1_die1_intClk_full_LN_2MHz_chn" + aschan[chan] + "_11_02_19_" + smin[chan] + "_data.csv";
    m_vinmin = -50.0;
    vinmax = 1750;
    m_nomVinPerAdc = 0.343;
  } else if ( ssam == "201611_p1_clklvd" ) {
    string smin[16] = {"11", "11", "12", "12", "12", "13", "13", "13",
                       "14", "14", "14", "15", "15", "15", "16", "16"};
    fname = m_topdir + "/201611/P1_die1/extClk_LVdefault_full_LN_2MHz/CSV/";
    fname += "P1_die1_extClk_LVdefault_full_LN_2MHz_chn" + aschan[chan] + "_11_02_20_" + smin[chan] + "_data.csv";
    m_vinmin = -50.0;
    vinmax = 1750;
    m_nomVinPerAdc = 1.0/2.933;
  } else if ( ssam == "201611_p1_clkfac" ) {
    string smin[16] = {"23", "23", "23", "24", "24", "24", "25", "25",
                       "25", "26", "26", "27", "27", "27", "28", "28"};
    fname = m_topdir + "/201611/P1_die1/extClk_factory_full_LN_2MHz/CSV/";
    fname += "P1_die1_extClk_factory_full_LN_2MHz_chn" + aschan[chan] + "_11_02_20_" + smin[chan] + "_data.csv";
    m_vinmin = -50.0;
    vinmax = 1750;
    m_nomVinPerAdc = 0.342;
  } else if ( ssam == "201611_p1_old" ) {
    string smin[16] = {"17", "18", "18", "19", "19", "19", "20", "20",
                       "20", "21", "21", "21", "22", "22", "22", "23"};
    fname = m_topdir + "/201611/P1_old/LN_2MHz_full/CSV/";
    fname += "P1_die1_extClk_factory_full_LN_2MHz_chn" + aschan[chan] + "_11_02_20_" + smin[chan] + "_data.csv";
    m_vinmin = -50.0;
    vinmax = 1750;
  } else if ( ssam == "201611_vstar" ) {
    string smin[16] = {"20", "20", "21", "21", "22", "22", "22", "23",
                       "23", "23", "24", "24", "24", "25", "25", "25"};
    fname = m_topdir + "/201611/Vstar_old/LN_2MHz/CSV/";
    fname += "Vstar_B-2_ADC6_LN_2MHz_chn" + aschan[chan] + "_10_28_17_" + smin[chan] + "_data.csv";
    m_vinmin = -50.0;
    vinmax = 1750;
    m_nomVinPerAdc = 1.0/2.316;
  } else if ( ssam == "201612_ltc2314" ) {
    if ( chan!=0 && chan!=1 && chan !=8 && chan!=11 ) return;
    fname = m_topdir + "/201612/ltc2314/LTC2314_raw_data/";
    fname += "ch" + aschan[chan] + "_360v_ltc2314_1_LN2_2M_006Hz_ramp_n01_23_sps_sps.csv";
    m_vinmin = -100.0;
    vinmax = 2300;
    m_nadc = 16384;
    isRaw = true;
    m_nomVinPerAdc = 0.1151;
  } else if ( ssam.substr(0, 10) == "201612_p1-" ) {     // 201612_p1-CB_clkTTT_FFFF_f3X
    string subdir;
    string schp;
    if ( ssam.size() == 28 ) {
      schp = ssam.substr(10,1);
      string sbos = ssam.substr(11,1);
      string sclk = ssam.substr(13,6);
      string sful = ssam.substr(20,4);
      string sf3x = ssam.substr(25,3);
      subdir = "LN2_2M_";
      if      ( sbos == "b" ) subdir += "brd";
      else if ( sbos == "s" ) subdir += "skt";
      else bad += 10;
      subdir += "_P1_" + schp;
      string sclksuf;
      string sfield = "_01";  // external
      if ( sclk == "clkint" ) sfield = "_00";
      else if ( sclk == "clkex1" ) sclksuf = "_clk1";
      else if ( sclk == "clkex2" ) sclksuf = "_clk2";
      else if ( sclk == "clkex3" ) sclksuf = "_clk3";
      else bad += 100;
      subdir += sfield;
      sfield = "";
      if      ( sful == "full" ) subdir += "_0";
      else if ( sful == "part" ) subdir += "_1";
      else bad += 1000;
      if      ( sf3x == "f30" ) subdir += "_0";
      else if ( sf3x == "f31" ) subdir += "_1";
      else bad += 10000;
      subdir += sclksuf;
      istringstream sschp(schp);
      sschp >> m_chip;
    } else bad = 1;
    if ( ! bad ) dirname = m_topdir + "/201612/p1_adc_old_format/P1_" + schp + "/" + subdir;
    m_vinmin = -50.0;
    vinmax = 1750;
    m_nomVinPerAdc = 1.0;
  // January 2017. a is the original files, b is after 20feb fix for offsets
  } else if ( ssam.substr(0, 6) == "201701" ) {     // 201701x_AA  x=a,b AA=die # (00, 02, 03, ...)
    string subdir;
    string schp;
    string topsubdir;
    if ( ssam.size() == 10 ) {
      schp = ssam.substr(8,2);
      if ( ssam[6] == 'a' ) {
        topsubdir = "/201701/P1_ADC_Data";
      } else if ( ssam[6] == 'b' ) {
        static std::set<string> update_schps = {"03", "06", "07", "17", "21", "22", "25", "26", "29", "32", "35"};
        if ( update_schps.find(schp) == update_schps.end() ) {
          topsubdir = "/201701/P1_ADC_Data";
        } else {
          topsubdir = "/201701/P1_ADC_Data_update_LN_2Msps";
        }
      } else {
        bad = 1;
      }
    } else {
      bad = 1;
    }
    if ( bad == 0 ) {
      dirname = m_topdir + topsubdir + "/P1_S7_" + schp;
      m_dataset = ssam.substr(0, 6);
      istringstream sschp(schp);
      sschp >> m_chip;
      m_vinmin = -300.0;
      vinmax = 1700;
      m_nomVinPerAdc = 1.0;
      schanPrefix = "LN_2MHz_chn";
      isRaw = true;
      m_dvdt = 200.0;
      m_nomVinPerAdc = 0.34;
    }
  } else {
    bad = 999;
  }
  if ( fname.size() == 0 ) {
    if ( dirname.size() == 0 ) {
      cout << myname << "Invalid sample name: " << ssam << endl;
      cout << myname << "Bad flag: " << bad << endl;
      return;
    }
    if ( gSystem->AccessPathName(dirname.c_str()) ) {
      cout << myname << "Unable to find directory " << dirname << endl;
      return;
    }
    FileDirectory filedir(dirname);
    string spat = schanPrefix + aschan[chan];
    FileDirectory::FileMap files = filedir.find(spat);
    if ( files.size() == 1 ) {
      fname = filedir.dirname + "/" + files.begin()->first;
    } else {
      cout << myname << "Unable to find file with pattern " << spat
           << " in directory " << filedir.dirname << endl;
      return;
    }
  }
  if ( m_nvin == 0 && vinmax > 0.0 ) m_nvin = (vinmax - m_vinmin + 0.01)/m_dvin;
  cout << myname << "File: " << fname << endl;
  
  vector<double> vvin(nvin(), -5000);
  for ( Index ivin=0; ivin<nvin(); ++ivin ) {
    vvin[ivin] = vinmin() + 0.1*ivin;
  }
  // Fetch the count for each ADC-Vin bin
  Index maxvin = 0;
  Index nline = 0;
  ifstream fin(fname.c_str());
  if ( ! fin ) {
    cout << myname << "Unable to open file " << fname << endl;
    return;
  }
  if ( nadc() <= 0.0 ) {
    cout << myname << "Invalid ADC code size " << nadc() << endl;
    return;
  }
  if ( nvin() <= 0 ) {
    cout << myname << "Invalid voltage bin count: " << nvin() << endl;
    return;
  }
  if ( dvin() <= 0.0 ) {
    cout << myname << "Invalid Vin bin size: " << dvin() << endl;
    return;
  }
  m_table.resize(nadc());
  if ( isRaw ) {
    cout << myname << "Reading raw data." << endl;
    if ( samplingFrequency() <= 0.0 ) {
      cout << myname << "Invalid sampling frequency: " << samplingFrequency() << endl;
      return;
    }
    if ( dvdt() <= 0.0 ) {
      cout << myname << "Invalid dVin/dt: " << dvdt() << endl;
      return;
    }
    double dv = 0.0;
    double dt = 1.0/samplingFrequency();  // sec
    Index isam = 0;
    for ( CountVector& vec : m_table ) vec.resize(nvin());
    while ( fin && ! fin.eof() ) {
      double iadc = 0.0;
      double t = isam*dt;
      double vin = dvdt()*t;
      Index ivin = vin/dvin();
      if ( ivin > nvin() ) {
        cout << myname << "Vin is too large: " << vin << endl;
        return;
      }
      string sline;
      getline(fin, sline);
      if ( ! fin ) {
        //cout << myname << "File read failed." << endl;
        break;
      }
      istringstream ssline(sline);
      ssline >> iadc;
      //cout << myname << "  ... " << isam << ": Vin=" << vin << ", ADC=" << iadc << endl;
      if ( iadc >= m_table.size() ) {
        cout << myname << "Table is too small for index " << iadc << endl;
        return;
      }
      if ( ivin >= m_table[iadc].size() ) {
        cout << myname << "Table row " << iadc << " is too small for index " << ivin << endl;
        cout << myname << "  Row size is " << m_table[iadc].size() << endl;
        cout << myname << "  Vin = " << vin << endl;
        return;
      }
      m_data.push_back(iadc);
      ++m_table[iadc][ivin];
      //if ( iadc == 3500 ) cout << myname << "  ... " << iadc << ", " << ivin << ": " << m_table[iadc][ivin] << endl;
      ++isam;
      if ( maxsam > 0 && isam >= maxsam ) {
        cout << myname << "Reached maximum sample count of " << maxsam << endl;
        break;
      }
    }
    maxvin = nvin();
    nline = isam;
    m_nsample = isam;
    cout << myname << "Waveform tick count: " << isam << endl;
  } else {
    cout << myname << "Reading table data." << endl;
    for ( Index iadc=0; iadc<nadc(); ++iadc ) {
      m_table[iadc].resize(nvin(), 0);
      string sline;
      getline(fin, sline);
      if ( ! fin ) {
        cout << myname << "File read failed." << endl;
        break;
      }
      istringstream ssline(sline);
      Index ivin;
      for ( Index ivin=0; ivin<nvin(); ++ivin ) {
        ssline >> m_table[iadc][ivin];
        m_nsample += m_table[iadc][ivin];
        if ( ! ssline ) {
          break;
        }
        if ( ivin >= maxvin ) maxvin = ivin+1;
        ssline.ignore(10, ',');
      }
      ++nline;
    }
  }
  cout << myname << "# ADC: " << nadc() << endl;
  cout << myname << "# Vin: " << nvin() << endl;
  cout << myname << "# lines: " << nline << endl;
  cout << myname << "Total count: " << nsample() << endl;
  cout << myname << "Total count/nvin: " << nsample()/nvin() << endl;
  cout << myname << "Total count/nadc: " << nsample()/nadc() << endl;
  return;
}

//**********************************************************************

double AdcSampleReader::vin(Index ivin) const {
  return vinmin() + dvin()*ivin;
}

//**********************************************************************
