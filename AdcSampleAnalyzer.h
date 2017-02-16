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
#include "FileDirectory.h"
#include <string>
#include "TH2.h"
#include "TF1.h"

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

public:

  AdcSampleReader reader;
  unsigned int adcUnderflow = 0;
  unsigned int adcOverflow = 4095;
  unsigned int minCountForStats = 2;
  TH2* phf = nullptr;   // Vin vs ADC without under, over and stuck bits
  TH2* phc = nullptr;   // Vin vs ADC
  TH2* phd = nullptr;   // ADC diff from linear fit
  TH2* phdw = nullptr;  // ADC diff from linear fit with broader range and coarser binning
  TH2* phn = nullptr;   // ADC diff from fixed-gain
  // Following are the stat histograms. One entry for each ADC code bin.
  // Bins with fewer than minCountForStats entries are recorded as underflows.
  TH1* phm = nullptr;   // Mean ADC diff
  TH1* phr = nullptr;   // Mean ADC RMS
  TH1* phs = nullptr;   // Mean ADC standard deviation
  TH1* phsg = nullptr;  // Mean ADC standard deviation for non-stuck codes
  TH1* phsb = nullptr;  // Mean ADC standard deviation for stuck codes
  TH1* phdn = nullptr;  // Mean ADC nominal RMS distribution
  TH1* phdr = nullptr;  // Mean ADC fitted RMS distribution
  TH1* phds = nullptr;  // Mean ADC fitted sigma distribution for ADC > 64, non stuck
  TH1* phdsb = nullptr; // Mean ADC fitted sigma distribution for ADC > 64, stuck
  TF1* pfit = nullptr;
  double fitVinPerAdc;
  double fitped;
  AdcChannelCalibration calib;
  AdcVoltageResponseVector voltageResponses;

  // Read in the data.
  //  asample - sample name
  //  achan - channel #
  //  maxsam - maximum # samples to read from a waveform (0 for all)
  //  nomGain - if nonzero, this value is used for the nominal gain [(ADC count/mV]
  AdcSampleAnalyzer(Name asample, Index achan, Index maxsam =0, double nomGain =0.0);

  // Return the distribution of Vin for an ADC count.
  // This projects phc onto the X axis.
  TH1* hcalib(unsigned int chan) const;

  // Return the nominal difference histogram for an ADC count.
  // This is the difference between the actual and common gain values for Vin.
  // This projects phn onto the X axis.
  TH1* hdiffn(unsigned int chan) const;

  // Return the difference histogram for an ADC count.
  // This is the difference between the actual and linear fit for Vin.
  // This projects phd onto the X axis.
  TH1* hdiff(unsigned int chan) const;

  // Return the distribution of Vin for an ADC count.
  // This is the same as hdiff except when there are underflows or overflows,
  // it uses the wider range and coarser bins from phdw.
  TH1* hdiffcalib(unsigned int chan) const;

  // Evaluate voltage responses.
  AdcVoltageResponseVector& evaluateVoltageReponses(double vmin, double vmax, Index nv);

};

#endif
