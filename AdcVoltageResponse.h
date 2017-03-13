// AdcVoltageResponse.h

// David Adams
// February 2017
//
// Class that records the ADC response to an input voltage range:
//    vmin <= Vin < vmax
// The response is the fraction of samples that fall into each ADC bin.

#ifndef AdcVoltageResponse_H
#define AdcVoltageResponse_H

#include <vector>
#include "AdcChannelId.h"

class AdcVoltageResponse {

public:

  using Index = unsigned int;
  using Float = float;
  using IndexVector = std::vector<Index>;
  using FloatVector = std::vector<Float>;

  // Ctors.
  AdcVoltageResponse();
  AdcVoltageResponse(AdcChannelId aid, Float avmin, Float avmax);

  // This object is valid if it holds a valid ID.
  bool isValid() const { return id().isValid() && vmax > vmin; }

  // Return the ADC channel ID.
  AdcChannelId id() const;

  // Return the # samples for a given bin number.
  Index count(Index ibin) const;

  // Return the fraction for a given bin number.
  // Returns 0 if not closed.
  float fraction(Index ibin) const;

  // Add an ADC sample.
  // 0 for success.
  // 1 if closed (sample is ignored).
  int addSample(Index adcbin, Index nsam =1);

  // Close. Removes leading empty bins, sets bin0 and calculates fractions.
  // 0 for success.
  // Returns 1 if no samples.
  // Returns 2 if already closed (no harm).
  int close();

public:

  Index chip;
  Index chan;
  Float vmin;
  Float vmax;
  Index bin0;             // First ADC bin index
  IndexVector counts;     // Count in each ADC bin.
  FloatVector fractions;  // Fraction of samples in each ADC bin.

};

#endif
