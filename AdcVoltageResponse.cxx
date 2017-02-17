// AdcVoltageResponse.cxx

#include "AdcVoltageResponse.h"

//**********************************************************************

AdcVoltageResponse::AdcVoltageResponse()
: chip(0), chan(0), vmin(0), vmax(0), bin0(0) { }

//**********************************************************************

AdcVoltageResponse::AdcVoltageResponse(AdcChannelId aid, Float avmin, Float avmax)
: chip(aid.chip), chan(aid.chan), vmin(avmin), vmax(avmax), bin0(0) { }

//**********************************************************************

AdcChannelId AdcVoltageResponse::id() const {
  return AdcChannelId(chip, chan);
}

//**********************************************************************

AdcVoltageResponse::Index AdcVoltageResponse::count(AdcVoltageResponse::Index ibin) const {
  if ( ibin < bin0 ) return 0;
  Index jbin = ibin - bin0;
  if ( jbin >= counts.size() ) return 0;
  return counts[jbin];
}

//**********************************************************************

float AdcVoltageResponse::fraction(AdcVoltageResponse::Index ibin) const {
  if ( fractions.size() == 0 ) return 0;
  if ( ibin < bin0 ) return 0;
  Index jbin = ibin - bin0;
  if ( jbin >= fractions.size() ) return 0;
  return fractions[jbin];
}

//**********************************************************************

int AdcVoltageResponse::
addSample(AdcVoltageResponse::Index ibin, AdcVoltageResponse::Index nsam) {
  if ( fractions.size() ) return 1;
  if ( ibin >= counts.size() ) counts.resize(ibin+1, 0);
  counts[ibin] += nsam;
  return 0;
}

//**********************************************************************

int AdcVoltageResponse::close() {
  if ( counts.size() == 0 ) return 1;
  if ( fractions.size() != 0 ) return 2;
  bin0 = 0;
  while ( bin0 < counts.size() && counts[bin0] == 0  ) ++bin0;
  Index sum = 0;
  for ( Index jbin=bin0; jbin<counts.size(); ++jbin ) {
    Index thiscount = counts[jbin];
    counts[jbin-bin0] = thiscount;
    sum += thiscount;
  }
  counts.resize(counts.size() - bin0);
  double xsum = sum;
  fractions.reserve(counts.size());
  for ( Index thiscount : counts ) {
    double thisfrac = thiscount/xsum;
    fractions.push_back(thisfrac);
  }
  return 0;
}

//**********************************************************************
