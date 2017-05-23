// AdcSampleAnalyzer.cxx
//
// David Adams
// February 2017
//
// Class to create histograms and calibration data for one ADC channel.

#ifndef adchist_H
#define adchist_H

#include "AdcSampleReader.h"
#include "AdcChannelCalibration.h"
#include "AdcVoltageResponse.h"
#include "AdcVoltagePerformance.h"
#include "FileDirectory.h"
#include <string>
#include <memory>
#include "TH2.h"
#include "TF1.h"
#include "TGraphAsymmErrors.h"

class TLine;

class AdcSampleAnalyzer {

public:

  using Name = std::string;
  using Index = unsigned int;
  using Count = unsigned int;
  using CountVector = std::vector<Count>;
  using CountTable = std::vector<CountVector>;

  using Code = unsigned short;
  using CodeVector = std::vector<Code>;
  using AdcVoltageResponseVector = std::vector<AdcVoltageResponse>;

  using AdcVoltagePerformanceVector = std::vector<AdcVoltagePerformance>;

  using TLineVector = std::vector<TLine*>;

  using AdcSampleReaderPtr = std::unique_ptr<AdcSampleReader>;

  using TH1Vector = std::vector<TH1*>;

public:

  unsigned int adcUnderflow = 0;
  unsigned int adcOverflow = 4095;
  unsigned int minCountForStats = 2;
  bool tailFracUsesPull = false;
  TH2* phf = nullptr;   // Vin vs ADC without under, over and stuck bits
  TH2* phc = nullptr;   // Vin vs ADC
  TH2* phd = nullptr;   // ADC diff from linear fit
  TH2* phdw = nullptr;  // ADC diff from linear fit with broader range and coarser binning
  TH2* phn = nullptr;   // ADC diff from nominal calibration
  TH2* phnw = nullptr;  // ADC diff from nominal calibration with broader range and coarser binning
  TH2* phvn = nullptr;  // ADC diff from nominal calibration vs Vin
  // Following are the stat histograms. One entry for each ADC code bin.
  // Bins with fewer than minCountForStats entries are recorded as underflows.
  TH1* phm = nullptr;   // Mean ADC diff vs ADC bin
  TH1* phr = nullptr;   // Mean ADC RMS vs ADC bin
  TH1* phs = nullptr;   // Mean ADC standard deviation vs ADC bin
  TH1* pht = nullptr;   // Tail fraction vs ADC bin
  TH1* phsx = nullptr;  // ADC expanded standard deviation (so pull <5)
  TH1* phsg = nullptr;  // Mean ADC standard deviation for classic non-stuck codes
  TH1* phsb = nullptr;  // Mean ADC standard deviation for classic stuck codes
  TH1* phdn = nullptr;  // Mean ADC nominal RMS distribution
  TH1* phdr = nullptr;  // Mean ADC fitted RMS distribution
  TH1* phds = nullptr;  // Mean ADC fitted sigma distribution for ADC > 64, classic non stuck
  TH1* phdsb = nullptr; // Mean ADC fitted sigma distribution for ADC > 64, classic stuck
  TF1* pfit = nullptr;
  Index iadcfitmin = 0;      // Min adc code for linear reponse fit
  Index iadcfitmax = 0;      // Max adc code for linear reponse fit
  double vinfitmin = 0.0;    // Min Vin for linear reponse fit
  double vinfitmax = 0.0;    // Max Vin for linear reponse fit
  bool fitusestuck = false;  // If true, classic stuck codes (LSB6=0,63) are excluded from linear response fit.
  // Nominal calibration.
  std::string datasetCalib;
  bool haveNominalCalibration = false;
  bool nominalCalibrationIsLinear = false;
  const AdcChannelCalibration* pcalNominal = nullptr;
  double nominalGain = 0.0;
  double nominalOffset = 0.0;
  // Linear fit.
  double fitGain;
  double fitOffset;
  AdcVoltageResponseVector voltageResponses;
  //std::vector<double> voltageEfficiencies;
  AdcVoltagePerformanceVector vperfs;
  TH1* phveff = nullptr;   // Efficiency vs Vin.
  TH1* phvrms = nullptr;   // Mean good RMS vs Vin.
  TH1* phvtail = nullptr;   // Tail fraction vs Vin.
  TGraphAsymmErrors* pgvrms = nullptr;
  TLineVector g80bars;
  // Threshold for pull fractions.
  double pullthresh = 5.0;
  // Threshold [mV] for tail window.
  double tailWindow = 5.0;
  bool evaluateReadData =false;   // Flag indicating if data was read for performance evaluation.

  // Read in and process the data using nominal calibration from adatasetCalib.
  // If the latter is "linear", the nominal calibration is linear with gain specified
  // by nomGain and the offset is obtained from the data at ADC=500.
  // If the string is "none" or blank, there is no nominal calibration and the
  // calibration difference histograms are not created. This saves memory.
  //   areader - Sample reader
  //   adatasetCalib - Name of the calibration dataset (e.g. "201701b").
  //   maxsam - maximum # samples to read from a waveform (0 for all)
  //  nomGain: >0 - Nominal gain for a linear calibration [(ADC count/mV]
  //            0   Take gain from reader
  AdcSampleAnalyzer(const AdcSampleReader& areader, Name adatasetCalib ="", double nomGain =0.0);

  // Same as previous except this object now manages the reader.
  // The reader will be deleted when this analyzer is deleted.
  // Caller must move the input pointer: AdcSampleyAnalyzer myobj(std::move(prdr), ...)
  AdcSampleAnalyzer(AdcSampleReaderPtr preader, Name adatasetCalib ="", double nomGain =0.0);

  // Construct analyzer from a channel calibration
  AdcSampleAnalyzer(Name a_dataset, Name a_sampleName, const AdcChannelCalibration& a_calib);

  // Dtor. Needed to delete locally managed histograms.
  ~AdcSampleAnalyzer();

  // Remove locally managed histograms and delete the reader if it is managed here.
  void clean();

  // Getters.
  const AdcSampleReader* reader() const { return m_preader; }
  Name dataset() const { return m_dataset; }
  Name sampleName() const { return m_sampleName; }
  const AdcChannelCalibration& calib() { return m_refCalib; }
  Index chip() const { return m_chip; }
  Index channel() const { return m_channel; }
  AdcTime time() const { return m_time; }
  Index nadc() const { return m_nadc; }

  // Return the distribution of Vin for an ADC count.
  // This projects phc onto the X axis.
  TH1* hcalib(unsigned int iadc) const;

  // Return the nominal difference histogram for an ADC count.
  // This is the difference between the actual and common gain values for Vin.
  // This projects phn onto the X axis.
  TH1* hdiffn(unsigned int iadc) const;

  // Return the difference histogram for an ADC count.
  // This is the difference between the actual and linear fit for Vin.
  // This projects phd onto the X axis.
  TH1* hdiff(unsigned int iadc) const;

  // Return the distribution of Vin for an ADC count.
  // This is the same as hdiff except when there are underflows or overflows,
  // it uses the wider range and coarser bins from phdw.
  TH1* hdiffcalib(unsigned int iadc) const;

  // Return the ultimate calibration (mean and RMS/sigma) for a bin.
  double calMean(Index iadc) const;
  double calRms(Index iadc) const;
  double calTail(Index iadc) const;
  double calExpandedRms(Index iadc) const;

  // Nominal calibration.
  double nominalCalibrationVin(Index iadc) const;
  double nominalCalibrationRms(Index iadc) const;

  // Evaluate voltage responses.
  AdcVoltageResponseVector& evaluateVoltageResponses(double vmin, double vmax, Index nv);

  // Evaluate the expected performance including
  //   efficiency = the fraction of samples that have RMS < rmsmax for each voltage bin.
  //   resolution = expected or actual deviation between input voltage and its calibration
  //   tail fraction = expected or actual fraction of samples with pull > 5.0
  // This adds an entry to vperfs.
  // If readData is true, the data is reread and used to calculate resolution and tail fraction
  const AdcVoltagePerformance::FloatVector& evaluateVoltageEfficiencies(double rmsmax, bool readData, bool dropTails =false);

  // Overlay efficiency, resolution and tail all vs. Vin.
  void drawperf(bool dolabtail =false) const;

  // Locally managed histograms.
  mutable TH1Vector m_localHists;
  
private:

  const AdcSampleReader* m_preader = nullptr;
  AdcSampleReaderPtr m_preaderManaged;
  AdcChannelCalibration m_localCalib;
  AdcChannelCalibration& localCalib() { return m_localCalib; }
  const AdcChannelCalibration& m_refCalib;
  Name m_dataset;
  Name m_sampleName;
  Index m_chip = badChip();
  Index m_channel = badChannel();
  AdcTime m_time = badTime();
  Index m_nadc = 0;
  SampleIndex nsample = 0;    // # ticks in sample

  int createHistograms(Index nvin, double vinmin, double vinmax);

};

#endif
