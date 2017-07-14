// AdcTreeChannelCalibration.h

// David Adams
// January 2017
//
// Class to hold the calibration data for an ADC channel.
//
// Linear calibration is
//   V = gain*code + offset
// where code is the ADC code (0-4095) and V and offset are in mV.
//
// Full calibrations is calMean(code) with RMS calRms(code). It is based
// on calCount(code) measurements with that code. Fraction in the tail
// of the distribution (e.g. pull > 5) is calTail(code).

#ifndef AdcTreeChannelCalibration_H
#define AdcTreeChannelCalibration_H

#include "AdcChannelCalibration.h"
#include <vector>
#include "AdcChannelId.h"
#include "AdcTypes.h"

class TTree;
class TFile;

// This is the data held in one ADC calibration tree entry.
class AdcTreeChannelCalibrationData {
public:
  AdcTreeChannelCalibrationData() = default;
  AdcTreeChannelCalibrationData(std::string asample,
                                ShortIndex achip, ShortIndex achan, AdcTime atime =badIndex(),
                                Float again =0.0, Float aoffset =0.0);
  AdcTreeChannelCalibrationData(std::string asample,
                                ShortIndex achip, ShortIndex achan, AdcTime atime,
                                Float again, Float aoffset,
                                const FloatVector& acalMeans,
                                const FloatVector& acalRmss,
                                const ShortIndexVector& acalCounts);
  std::string sample;
  ShortIndex chip = badShortIndex();
  ShortIndex chan = badShortIndex();
  AdcTime time = badIndex();
  Float gain =0.0;
  Float offset =0.0;
  FloatVector calMeans;
  FloatVector calRmss;
  FloatVector calTails;
  ShortIndexVector calCounts;
};

class AdcTreeChannelCalibration : public AdcChannelCalibration {

public:

  // Sublass that holds pointers to a tree and its file and closes
  // the file when it is deleted.
  class TreeWrapper {
  public:
    TreeWrapper(TTree* aptree, TFile* apfile) : m_ptree(aptree), m_pfile(apfile) { }
    ~TreeWrapper();
    TTree* tree() { return m_ptree; }
  private:
    TTree* m_ptree;
    TFile* m_pfile;
  };

  // Find the tree for a given dataset name.
  static TreeWrapper findTree(std::string dataset);

  // Load an existing calibration from a Root TTree.
  static const AdcTreeChannelCalibration* find(std::string dataset, std::string asample);
  static const AdcTreeChannelCalibration* find(std::string dataset, AdcChannelId aid);
  static const AdcTreeChannelCalibration* find(std::string dataset, ShortIndex chip, ShortIndex chan);

  // Ctors.
  AdcTreeChannelCalibration();
  AdcTreeChannelCalibration(std::string asample, AdcChannelId aid, AdcTime atime);
  AdcTreeChannelCalibration(std::string asample, AdcChannelId aid, AdcTime atime,
                            Float again, Float aoffset,
                            const FloatVector& acalMeans,
                            const FloatVector& acalRmss,
                            const ShortIndexVector& acalCounts);
  AdcTreeChannelCalibration(const AdcTreeChannelCalibrationData& dat);
  AdcTreeChannelCalibration(const AdcTreeChannelCalibration& cal);
  AdcTreeChannelCalibration(AdcTreeChannelCalibration&& cal) = default;

  // Copy.
  AdcTreeChannelCalibration& operator=(const AdcTreeChannelCalibration& cal);

  // Set the calibration name.
  void setName(std::string cname);

  // Return the ADC channel ID.
  AdcChannelId id() const { return AdcChannelId(chip(), channel()); }

  // This object is valid if it holds a valid ID.
  bool isValid() const override { return id().isValid(); }

  // Full calibration for each ADC code.
  std::string name() const override;
  std::string sample() const override;
  Index chip() const override;
  Index channel() const override;
  Index time() const override;
  Index size() const override;
  double calMean(AdcCode code) const override;
  double calRms(AdcCode code) const override;
  double calTail(AdcCode code) const override;
  double linearOffset() const override;
  double linearGain() const override;
  ShortIndex calCount(AdcCode code) const;

  // Full calibration data (tree entry).
  AdcTreeChannelCalibrationData& data() { return m_data; }
  const AdcTreeChannelCalibrationData& data() const { return m_data; }

public:

  std::string m_name = "TreeCalibration";
  AdcTreeChannelCalibrationData m_data;

};

#endif
