// AdcChipAnalyzer.h

// Script that analyzes data for channels on an ADC chip.
// It provides options to fill calibration and performance trees and
// to make plots.
#ifndef AdcChipAnalyzer_H
#define AdcChipAnalyzer_H

#include <string>
#include <map>
#include "AdcSampleReader.h"

class AdcSampleAnalyzer;
class AdcChannelCalibration;
class TH1;
class TCanvas;

class AdcChipAnalyzer {

public:

  using Name = std::string;
  using TH1Vector = std::vector<TH1*>;

  // List the supported plots.
  static void help(Name prefix ="");

  // Ctor from the parameters used to create and update the sample analyzers.
  //   sampleName: chip sample name, e.g. 201701_14
  //   icha1, ncha: The channel range is ncha channels starting at icha1.
  //   datasetCalib: name of dataset holding the reference calibration
  //   saveCalib: If true, the calibration tree is updated for these channels
  //   vmin, vmax, nv: if nv>0 and vmax > vmin, then evalute voltage responses
  //                   (fraction of samples populating each ADC bin)
  //   vRmsMax: max RMS for voltage efficiency plots
  //   dropTails: Bins with tails are considereed bad if thsi is true.
  //   savePerf: If true, the performance tree is updated for these channels
  //   cleanFlag: 0 - Do not clean (delete reader after reading).
  //              1 - Clean. Saves a lot of memory.
  //              2 - Clean iff ncha > 1.
  // See AdcSampleAnalyzer for more info on these arguments.
  // Analyzers are created for the specified range and that range becomes the
  // default for plotting.
  AdcChipAnalyzer(std::string sampleName,
                  Index icha1 =0,
                  Index ncha =1,
                  std::string datasetCalib ="",
                  bool saveCalib =false,
                  float vmin = 0.0,
                  float vmax = 0.0,
                  unsigned int nv = 0,
                  double vRmsMax = 1.0,
                  bool dropTails = false,
                  bool savePerf =false,
                  int clean = 2);
  AdcChipAnalyzer(std:: string sopt, std::string sampleName,
                  Index icha1 =0,
                  Index ncha =1,
                  std::string datasetCalib ="",
                  bool saveCalib =false,
                  float vmin = 0.0,
                  float vmax = 0.0,
                  unsigned int nv = 0,
                  double vRmsMax = 1.0,
                  bool dropTails = false,
                  bool savePerf =false,
                  int clean = 2);

  ~AdcChipAnalyzer();

  // Set the channels to include in plots.
  // Reads data for channels as needed.
  // Arguments have the same meaning as in the ctor.
  int setChannels(Index icha1, Index ncha =1, int cleanFlag = 2);

  // Return the the analyzer for channel icha). Created if absent.
  AdcSampleAnalyzer* sampleAnalyzer(Index icha);

  // Draw a selected set of plots.
  int drawall();

  // Draw a plot. Use splot = "help" to list available plots wiht descriptions.
  int draw(Name splot, bool savePlot =false);

  // Getters.
  Name sampleName()    const { return m_sampleName; }
  Index firstChannel() const { return m_icha1; }
  Index nChannel()     const { return m_ncha; }
  Name datasetCalib()  const { return m_datasetCalib; }
  bool saveCalib()     const { return m_saveCalib; }
  double vmin()        const { return m_vmin; };
  double vmax()        const { return m_vmax; };
  Index nv()           const { return m_nv; };
  double vRmsMax()     const { return m_vRmsMax; };
  bool dropTails()     const { return m_dropTails; };
  bool savePerf()      const { return m_savePerf; };
  bool clean()         const { return m_clean; }
  TH1* gainHist()      const { return m_phGain; }
  TH1* offsetHist()    const { return m_phOffset; }
  const TH1Vector& rawHists() const { return m_rawHists; }
  const TH1Vector& ramHists() const { return m_ramHists; }
  const TH1Vector& vinHists() const { return m_vinHists; }

  // Return the channel number for a channel index.
  // Return nChannel for an invalid index.
  Index channel(Index kcha) const;

  // Return a new canvas;
  // This will have 1, 4 or 16 drawing pads depending on the drawing type (stype)
  // and # channels.
  TCanvas* newCanvas(Name splot, Name canName) const;

  bool fixped = true;

private:

  Name   m_sopt;
  Name   m_sampleName;
  Index  m_icha1;
  Index  m_ncha;
  Index  m_layout;   // 10, 41, 42, 43, 44, 160
  Name   m_datasetCalib;
  bool   m_saveCalib;
  double m_vmin;
  double m_vmax;
  Index  m_nv;
  double m_vRmsMax;
  bool   m_dropTails;
  bool   m_savePerf;
  bool   m_clean;
  std::vector<bool> m_haveAsa;
  std::vector<AdcSampleAnalyzer*> m_asas;
  std::vector<AdcChannelCalibration*> m_pcals;
  std::vector<Index> m_chans;
  TH1Vector m_hists;
  TH1Vector m_rawHists;
  TH1Vector m_ramHists;
  TH1Vector m_vinHists;
  std::vector<TCanvas*> m_cans;
  TH1* m_phGain;
  TH1* m_phOffset;
  std::map<string, TH1*> m_hsums;  // Summed histograms.
  bool m_calculateVin;

};

#endif
