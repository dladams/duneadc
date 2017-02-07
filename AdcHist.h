// AdcHist.cxx
//
// David Adams
// November 2016
//
// Class to create histograms and calibration data for one ADC channel.

#ifndef AdcHist_H
#define AdcHist_H

#include "AdcChannelCalibration.h"
#include "FileDirectory.h"
#include <string>
#include "TH2.h"
#include "TF1.h"

class AdcHist {

public:

  std::string dataset;   // Name of the dataset holding this ADC.
  unsigned int chip;     // ADC chip number in the datset.
  unsigned int channel;  // ADC channel number.
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

  // Build histograms and calibrations for one ADC channel.
  AdcHist(std::string ssam, int chan, double cfac =0.0);

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

};

#endif
