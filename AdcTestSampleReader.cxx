// AdcTestSampleReader.cxx

#include "AdcTestSampleReader.h"
#include <string>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include "TSystem.h"
#include "TH1F.h"
#include "FileDirectory.h"

using std::string;
using std::cout;
using std::endl;
using std::ifstream;
using std::ostringstream;
using std::getline;

using Name = AdcTestSampleReader::Name;
using Index = AdcTestSampleReader::Index;

//**********************************************************************

AdcTestSampleReader::AdcTestSampleReader(Name ssam)
: m_topdir(gSystem->ExpandPathName("$HOME/data/dune/adc")),
  m_sample(ssam),
  m_chip(badChip()),
  m_chan(badChannel()),
  m_nadc(4096),
  m_maxSample(0),
  m_nsample(0),
  m_nvin(0),
  m_dvin(0.1),
  m_vinmin(0.0),
  m_samplingFrequency(2.e6),
  m_dvdt(0.0),
  m_dvds(0.0),
  m_nomVinPerAdc(0.0) { }

//**********************************************************************

AdcTestSampleReader::AdcTestSampleReader(Name ssam, Index icha, Index maxsam)
: AdcTestSampleReader(ssam) {
  m_maxSample = maxsam;
  setChannel(icha);
}

//**********************************************************************

int AdcTestSampleReader::setChannel(Index icha) {
  if ( icha == m_chan ) return 0;
  m_chan = icha;
  return read();
}

//**********************************************************************

int AdcTestSampleReader::read() {
  const string myname = "AdcTestSampleReader::read: ";
  string fname;
  string dirname;
  const string aschanold[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15"};
  const string aschan[16] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F"};
  Index chan = channel();
  string schan = aschanold[chan];
  string ssam = sample();
  cout << myname << "Sample " << ssam << " channel " << chan << endl;
  bool isRawCsv = false;   // True if the input data is a CSV waveform
  bool isRawBin = false;   // True if the input data is a binary waveform
  bool isTable = false;    // True if input data is a CSV table.
  double vinmax = 0.0;  // If nvin==0 and vinmax>0, then nvin is calculated from vinmax
  int bad = 0;
  string schanPrefix;
  if ( ssam == "201610_vstar" ) {
    fname = m_topdir + "/201610/VSTAR_1S/LN2_vstar_1s_200MHz_vstar_adc_09_28_2016_15_03_35/";
    fname += "LN2_vstar_1s_200MHz_vstar_adc_09_28_2016_15_03_35_chn" + aschanold[chan] + ".csv";
    m_nomVinPerAdc = 1.0/2.346;
    isTable = true;
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
    isTable = true;
  } else if ( ssam == "201611_p1_clklvd" ) {
    string smin[16] = {"11", "11", "12", "12", "12", "13", "13", "13",
                       "14", "14", "14", "15", "15", "15", "16", "16"};
    fname = m_topdir + "/201611/P1_die1/extClk_LVdefault_full_LN_2MHz/CSV/";
    fname += "P1_die1_extClk_LVdefault_full_LN_2MHz_chn" + aschan[chan] + "_11_02_20_" + smin[chan] + "_data.csv";
    m_vinmin = -50.0;
    vinmax = 1750;
    m_nomVinPerAdc = 1.0/2.933;
    isTable = true;
  } else if ( ssam == "201611_p1_clkfac" ) {
    string smin[16] = {"23", "23", "23", "24", "24", "24", "25", "25",
                       "25", "26", "26", "27", "27", "27", "28", "28"};
    fname = m_topdir + "/201611/P1_die1/extClk_factory_full_LN_2MHz/CSV/";
    fname += "P1_die1_extClk_factory_full_LN_2MHz_chn" + aschan[chan] + "_11_02_20_" + smin[chan] + "_data.csv";
    m_vinmin = -50.0;
    vinmax = 1750;
    m_nomVinPerAdc = 0.342;
    isTable = true;
  } else if ( ssam == "201611_p1_old" ) {
    string smin[16] = {"17", "18", "18", "19", "19", "19", "20", "20",
                       "20", "21", "21", "21", "22", "22", "22", "23"};
    fname = m_topdir + "/201611/P1_old/LN_2MHz_full/CSV/";
    fname += "P1_die1_extClk_factory_full_LN_2MHz_chn" + aschan[chan] + "_11_02_20_" + smin[chan] + "_data.csv";
    m_vinmin = -50.0;
    vinmax = 1750;
    isTable = true;
  } else if ( ssam == "201611_vstar" ) {
    string smin[16] = {"20", "20", "21", "21", "22", "22", "22", "23",
                       "23", "23", "24", "24", "24", "25", "25", "25"};
    fname = m_topdir + "/201611/Vstar_old/LN_2MHz/CSV/";
    fname += "Vstar_B-2_ADC6_LN_2MHz_chn" + aschan[chan] + "_10_28_17_" + smin[chan] + "_data.csv";
    m_vinmin = -50.0;
    vinmax = 1750;
    m_nomVinPerAdc = 1.0/2.316;
    isTable = true;
  } else if ( ssam == "201612_ltc2314" ) {
    if ( chan!=0 && chan!=1 && chan !=8 && chan!=11 ) return 1;
    fname = m_topdir + "/201612/ltc2314/LTC2314_raw_data/";
    fname += "ch" + aschan[chan] + "_360v_ltc2314_1_LN2_2M_006Hz_ramp_n01_23_sps_sps.csv";
    m_vinmin = -100.0;
    vinmax = 2300;
    m_nadc = 16384;
    isRawCsv = true;
    m_nomVinPerAdc = 0.1151;
    m_dvdt = 2400.0/(1.0/0.06);
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
    isTable = true;
  // 201701a_CC - January 2017 chip CC original files for 25 chips CC=00-35
  // 201701b_CC - Same as a after 20feb fix for voltage calibration offsets (chip 6 is now broken and remains unclaibrated)
  // 201701c_CC - Same as b after mod to match feb data (chips 2 and 4 only)
  // 201701d_CC - January 2017 chip CC original files 1 MHz (voltage offsets  not fixed)
  // 201702DD_CC - Long-term data taken on day DD for chip CC=02
  // 201703a_DCC - March 2017 chip DCC calibrated files D00-D479 for 77 new chips
  } else if ( ssam.substr(0, 4) == "2017" ) {     // 201701x_AA  x=a,b AA=die # (00, 02, 03, ...)
    string subdir;
    string schp;
    string topsubdir;
    schanPrefix = "LN_2MHz_chn";
    m_dvdt = 200.0;
    m_vinmin = -300.0;
    vinmax = 1700;
    if ( ssam.substr(0, 6) == "201701" ) {;
      if ( ssam.size() == 10 ) {
        m_dataset = ssam.substr(0, 7);
        schp = ssam.substr(8,2);
        if ( ssam[6] == 'a' ) {
          topsubdir = "/201701/P1_ADC_Data";
        } else if ( ssam[6] == 'd' ) {
          topsubdir = "/201701/P1_ADC_Data";
          schanPrefix = "LN_1MHz_chn";
          m_dvdt = 400.0;
        } else if ( ssam[6] == 'b' ) {
          static std::set<string> update_schps_part1 =   {"03", "07", "21", "25", "26", "32", "35"};
          static std::set<string> update_schps_part2 =   {"17", "22", "29"};
          static std::set<string> update_schps_missing = {"06"};
          if        ( update_schps_part1.find(schp) != update_schps_part1.end() ) {
            topsubdir = "/201701/P1_ADC_Data_update_LN_2Msps";
          } else if ( update_schps_part2.find(schp) != update_schps_part2.end() ) {
            topsubdir = "/201701/P1_ADC_Data_update_LN_2Msps_part2";
          } else if ( update_schps_missing.find(schp) != update_schps_missing.end() ) {
            cout << myname << "WARNING: Chip 6 is now broken. Using old data." << endl;
            topsubdir = "/201701/P1_ADC_Data";
          } else {
            topsubdir = "/201701/P1_ADC_Data";
          }
        } else if ( ssam[6] == 'c' ) {
          string sday = "01";
          if ( schp == "02" ) sday = "0119";
          if ( schp == "04" ) sday = "0124";
          dirname = m_topdir + "/201702/P1_ADC_Long_Term_Test_Data/P1_S7_" + schp + "_" + sday;
        } else {
          bad = 1;
        }
        if ( dirname.size() == 0 ) dirname = m_topdir + topsubdir + "/P1_S7_" + schp;
      } else {
        bad = 1;
      }
    } else if ( ssam.substr(0, 6) == "201702" ) {
      if ( ssam.size() == 11 ) {
        string sday = ssam.substr(4,4);
        m_dataset = ssam.substr(0, 8);
        schp = ssam.substr(9,2);
        dirname = m_topdir + "/201702/P1_ADC_Long_Term_Test_Data/P1_S7_" + schp + "_" + sday;
      } else {
        bad = 1;
      }
    } else if ( ssam.substr(0, 8) == "20170306" ) {        // 20170306_CC
      m_dataset = ssam.substr(0, 8);
      schp = ssam.substr(9,2);
      dirname = m_topdir + "/20170307/P1_ADC_Long_Term_0306/P1_S7_02_1";
    } else if ( ssam.substr(0, 8) == "20170307" ) {        // 20170307_CC,
      m_dataset = ssam.substr(0, 8);
      schp = ssam.substr(9,2);
      dirname = m_topdir + "/20170307/P1_ADC_Long_Term_0306/P1_S7_02_2";
    } else if ( ssam.substr(0, 7) == "201703a" ) {
      m_vinmin = -200.0;
      vinmax = 1800;
      m_dvdt = 400.0;
      if ( ssam.size() == 11 ) {
        m_dataset = ssam.substr(0, 7);
        schp = ssam.substr(9,2);
        dirname = m_topdir + "/201703/P1_ADC_ScreeningData_03222017/P1_S7_D" + schp;
      } else {
        bad = 1;
      }
    } else {
      bad = 1;
    }
    if ( bad == 0 ) {
      istringstream sschp(schp);
      sschp >> m_chip;
      isRawCsv = true;
      m_nomVinPerAdc = 0.34;
    }

  } else {
    bad = 999;
  }
  if ( fname.size() == 0 ) {
    if ( dirname.size() == 0 ) {
      cout << myname << "ERROR: Invalid sample name: " << ssam << endl;
      cout << myname << "ERROR: Bad flag: " << bad << endl;
      return 2;
    }
    if ( gSystem->AccessPathName(dirname.c_str()) ) {
      cout << myname << "ERROR: Unable to find directory " << dirname << endl;
      return 3;
    }
    FileDirectory filedir(dirname);
    string spat = schanPrefix + aschan[chan];
    FileDirectory::FileMap files = filedir.find(spat);
    if ( files.size() == 1 ) {
      fname = filedir.dirname + "/" + files.begin()->first;
    } else {
      cout << myname << "ERROR: Unable to find file with pattern " << spat
           << " in directory " << filedir.dirname << endl;
      return 4;
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
    cout << myname << "ERROR: Unable to open file " << fname << endl;
    return 5;
  }
  if ( nadc() <= 0.0 ) {
    cout << myname << "ERROR: Invalid ADC code size " << nadc() << endl;
    return 6;
  }
  if ( nvin() <= 0 ) {
    cout << myname << "ERROR: Invalid voltage bin count: " << nvin() << endl;
    return 7;
  }
  if ( dvin() <= 0.0 ) {
    cout << myname << "ERROR: Invalid Vin bin size: " << dvin() << endl;
    return 8;
  }
  m_table.clear();
  m_table.resize(nadc());
  if ( isRawCsv ) {
    cout << myname << "Reading raw data." << endl;
    if ( samplingFrequency() <= 0.0 ) {
      cout << myname << "ERROR: Invalid sampling frequency: " << samplingFrequency() << endl;
      return 9;
    }
    if ( dvdt() <= 0.0 ) {
      cout << myname << "ERROR: Invalid dVin/dt: " << dvdt() << endl;
      return 10;
    }
    Index maxsam = maxSample();
    cout << myname << "Max # samples: " << maxsam << endl;
    double dv = 0.0;
    double dt = 1.0/samplingFrequency();  // sec
    Index isam = 0;
    // Read waveform.
    m_data.clear();
    while ( fin && ! fin.eof() ) {
      Code iadc = 0.0;
      string sline;
      getline(fin, sline);
      if ( ! fin ) {
        //cout << myname << "File read failed." << endl;
        break;
      }
      istringstream ssline(sline);
      ssline >> iadc;
      //cout << myname << "  ... " << isam << ": Vin=" << vin << ", ADC=" << iadc << endl;
      m_data.push_back(iadc);
    }
    cout << myname << "Waveform tick count: " << m_data.size() << endl;
    // Construct table from waveform.
    for ( CountVector& vec : m_table ) vec.resize(nvin());
    for ( Code iadc : m_data ) {
      double t = isam*dt;
      double vin = dvdt()*t;
      Index ivin = vin/dvin();
      if ( ivin > nvin() ) {
        cout << myname << "ERROR: Vin is too large: " << vin << endl;
        return 11;
      }
      if ( iadc >= m_table.size() ) {
        cout << myname << "ERROR: Table is too small for index " << iadc << endl;
        return 12;
      }
      if ( ivin >= m_table[iadc].size() ) {
        cout << myname << "ERROR: Table row " << iadc << " is too small for index " << ivin << endl;
        cout << myname << "ERROR:   Row size is " << m_table[iadc].size() << endl;
        cout << myname << "ERROR:   isam = " << isam << endl;
        cout << myname << "ERROR:   Vin = " << vin << endl;
        return 13;
      }
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
  m_dvds = m_dvdt;
  if ( samplingFrequency() > 0.0 ) {
    m_dvds /= samplingFrequency();
  }
  cout << myname << "# ADC: " << nadc() << endl;
  cout << myname << "# Vin: " << nvin() << endl;
  cout << myname << "# lines: " << nline << endl;
  cout << myname << "Total count: " << nsample() << endl;
  cout << myname << "Total count/nvin: " << nsample()/nvin() << endl;
  cout << myname << "Total count/nadc: " << nsample()/nadc() << endl;
  cout << myname << "Ramp rate: " << dvdt() << " mV/sec" << endl;
  return 0;
}

//**********************************************************************

double AdcTestSampleReader::vinLow(Index ivin) const {
  return vinmin() + dvin()*ivin;
}

//**********************************************************************

double AdcTestSampleReader::vinCenter(Index ivin) const {
  return vinmin() + dvin()*(ivin+0.5);
}

//**********************************************************************
