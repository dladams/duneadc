// AdcChipAnalyzer.h

// Script that creates the plots and fills calib tree for one
// or more channel in a chip.
//   ssam: chip sample name, e.g. 201701_14
//   icha1: First channel to process
//   ncha: # channels to process
//   datasetCalib: name of dataset holding the reference calibration
//   savecalib: If true, the calibration tree is updated for these channels
//   vmin, vmax, nv: if nv>0 and vmax > vmin, then evalute voltage responses
//                   (fraction of samples populating each ADC bin)
//   vrmsmax: max RMS for voltage efficiency plots

#ifndef AdcChipAnalyzer_H
#define AdcChipAnalyzer_H

#include <string>

class AdcSampleAnalyzer;

class AdcChipAnalyzer {

public:

  AdcChipAnalyzer(std::string ssam,
                  unsigned int icha1 =0,
                  unsigned int ncha =0,
                  std::string datasetCalib ="",
                  bool savecalib =false,
                  float vmin = 0.0,
                  float vmax = 0.0,
                  unsigned int nv = 0,
                  double vrmsmax = 1.0,
                  bool dropTails = false,
                  bool saveperf =false);

  ~AdcChipAnalyzer();

  // Pointer to the analyzer for each channel.
  // Null or absent if channel was not analyzed.
  std::vector<AdcSampleAnalyzer*> asas;

};

#endif
