// AdcSampleAnalyzer.cxx
//
// David Adams
// February 2017
//
// Class to create histograms and calibration data for one ADC channel.

#ifndef adchist_H
#define adchist_H

#include "AdcSampleReader.h"
#include "AdcTreeChannelCalibration.h"
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

  // Configuration parameters.
  unsigned int adcUnderflow = 0;
  unsigned int adcOverflow = 4095;
  unsigned int minCountForStats = 2;
  bool tailFracUsesPull = false;
  Index iadcfitmin = 0;        // Min adc code for linear reponse fit
  Index iadcfitmax = 0;        // Max adc code for linear reponse fit
  double vinfitmin = 0.0;      // Min Vin for linear reponse fit
  double vinfitmax = 0.0;      // Max Vin for linear reponse fit
  bool fitusestuck = false;    // If true, exclude classic stuck codes (LSB6=0,63) in linear fit
  double pullthresh = 5.0;     // Threshold for identifying tails with pull
  double tailWindow = 5.0;     // Threshold [mV] for identifying tails w/o pull.
  double pedCorVin = 300.0;    // Input voltage at which pedestal correction is made.
  Index pedCorHalfWindow = 20; // Pedestal correction evaluation range is +- this value.
  bool doUpDownFits =true;     // If true and tables are present, up and down fits are done

  // Output histograms.
  // Except phvn, the following are all vs. ADC bin
  TH2* phc = nullptr;   // Vin
  TH2* phf = nullptr;   // Vin without under, over and stuck bits (used in linear fit)
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
  TH1* phns = nullptr;  // Mean ADC nominal calibration RMS distribution
  TH1* phdr = nullptr;  // Mean ADC fitted RMS distribution
  TH1* phds = nullptr;  // Mean ADC fitted sigma distribution for ADC > 64, classic non stuck
  TH1* phdsb = nullptr; // Mean ADC fitted sigma distribution for ADC > 64, classic stuck

  // Linear response fit.
  TF1* pfit = nullptr;
  double fitGain = 0.0;
  double fitOffset = 0.0;

  // Linear response hists and fits separate for up and down slopes.
  // Thees are only filled if doUpDownFits is set true.
  bool haveUpDownHists = false;
  TH2* phfu = nullptr;  // Fit distribution for dVin/dt > 0
  TH2* phfd = nullptr;  // Fit distribution for dVin/dt < 0
  TF1* pfitU = nullptr;
  double fitGainU = 0.0;
  double fitOffsetU = 0.0;
  TF1* pfitD = nullptr;
  double fitGainD = 0.0;
  double fitOffsetD = 0.0;

  // Input and nominal calibration.
  // The nominal calibration is used to evaluate calibration difference histograms and
  // performance as a function of input voltage.
  // The nominal calibration is eitehr the same as the input or may be a pedestal
  // correction to the input.
  // In the latter case, the correction is done at Vin = pedCorVin using bins within
  // +/-pedCorHalfWindow of the corresponding ADC bin.
  // The input calibration is owned by the caller and should not be deleted before all
  // evaluation of calib diffs and performance have been completed.
  // The pedestal-corrected calibration is deleted when this object is deleted.
  const AdcChannelCalibration* pcalInput = nullptr;
  const AdcChannelCalibration* pcalNominal = nullptr;

  // Perfomance results. All in the same input voltage bins.
  AdcVoltageResponseVector voltageResponses;   // # of samples in each ADC bin
  AdcVoltagePerformanceVector vperfs;    // Efficiency and RMS deviation
  TH1* phveff = nullptr;   // Efficiency (fraction of samples in good ADC bins) vs Vin.
  TH1* phvdev = nullptr;   // Mean calibration deviation for good bins.
  TH1* phvrms = nullptr;   // RMS of the calibration deviation for good bins.
  TH1* phvadv = nullptr;   // Abs of the mean calibration deviation for good bins.
  TH1* phvtail = nullptr;  // Tail fraction vs Vin.
  TLineVector g80bars;     // (10,90)% range for the calibratin RMS
  TLineVector g100bars;    // (0,100)% range for the calibration RMS

  // Threshold for pull fractions.
  bool evaluateReadData =false;   // Flag indicating if data was read for performance evaluation.

  // Read in and process the data using calibration from pcal.
  // If there is no calibration (pcal is null), then the calibration difference
  // histograms are not created. This saves memory.
  //   areader - Sample reader
  //   pcal - Pointer to a reference calibration (for histograms phn, etc.)
  //   fixped - If true the ref calib is adjusted to have the same response near bin 300
  AdcSampleAnalyzer(const AdcSampleReader& areader,
                    const AdcChannelCalibration* pcal =nullptr,
                    bool fixped =false);

  // Same as previous except this object now manages the reader.
  // The reader will be deleted when this analyzer is deleted.
  // Caller must move the input pointer: AdcSampleyAnalyzer myobj(std::move(prdr), ...)
  AdcSampleAnalyzer(AdcSampleReaderPtr preader,
                    const AdcChannelCalibration* pcal =nullptr,
                    bool fixped =false);

  // Construct analyzer from a channel calibration.
  // The dataset and sample names are needed for labels.
  // If dataset is blank, it is the sample name truncated at the first underscore.
  AdcSampleAnalyzer(const AdcChannelCalibration& a_calib,
                    Name a_sampleName ="TestSample",
                    Name a_dataset ="");

  // Dtor. Needed to delete locally managed histograms.
  ~AdcSampleAnalyzer();

  // Remove locally managed histograms and delete the reader if it is managed here.
  void clean();

  // Getters.
  const AdcSampleReader* reader() const { return m_preader; }
  Name dataset() const { return m_dataset; }
  Name sampleName() const { return m_sampleName; }
  AdcTreeChannelCalibration& localCalib() { return m_localTreeCalib; }
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
  double nominalCalibrationTail(Index iadc) const;

  // Evaluate voltage responses.
  AdcVoltageResponseVector& evaluateVoltageResponses(double vmin, double vmax, Index nv);

  // Evaluate the expected performance including
  //   efficiency = the fraction of samples that have RMS < rmsmax for each voltage bin.
  //   resolution = expected or actual deviation between input voltage and its calibration
  //   tail fraction = expected or actual fraction of samples with pull > 5.0
  // This adds an entry to vperfs.
  // If readData is true, the data is reread and used to calculate resolution and tail fraction
  const AdcVoltagePerformance::FloatVector&
  evaluateVoltageEfficiencies(double rmsmax, bool readData, bool dropTails =false);

  // Overlay efficiency, resolution and tail all vs. Vin.
  // Returns 0 for success.
  int drawperf(bool dolabtail =false) const;

private:

  const AdcSampleReader* m_preader = nullptr;
  AdcSampleReaderPtr m_preaderManaged;
  AdcTreeChannelCalibration m_localTreeCalib;     // The calibration created by this analyzer.
  const AdcChannelCalibration& m_refCalib;
  Name m_dataset;
  Name m_sampleName;
  Index m_chip = badChip();
  Index m_channel = badChannel();
  AdcTime m_time = badTime();
  Index m_nadc = 0;
  SampleIndex nsample = 0;    // # ticks in sample

  // Locally managed histograms that are deleted when this object is cleaned.
  mutable TH1Vector m_cleanHists;
  
  // Locally managed histograms that are deleted when this object is deleted.
  mutable TH1Vector m_saveHists;
  
  // Manage a histogram.
  // If cleaned is true, the histogram is deleted when cleaning is done.
  // Otherwise is is kept until this object is deleted.
  void manageHist(TH1* ph, bool cleaned =false) const;
  void saveHist(TH1* ph) const  { manageHist(ph, false); }
  void cleanHist(TH1* ph) const { manageHist(ph, true); }

  int createHistograms(Index nvin, double vinmin, double vinmax);
  TH1* createManagedHistogram(Name name, Name title,
                              Index nx, double xmin, double xmax,
                              Index ny =0, double ymin =0.0, double ymax =-1.0);
  TH2* createManaged2dHistogram(Name name, Name title,
                                Index nx, double xmin, double xmax,
                                Index ny, double ymin, double ymax);

};

#endif
