// AdcChannelId.h

// David Adams
// January 2017
//
// Class to hold the identifier for an ADC channel.

#ifndef AdcChannelId_H
#define AdcChannelId_H

class AdcChannelId {

public:

  using Index = unsigned short;

  AdcChannelId() =default;
  AdcChannelId(Index achip, Index achan) : chip(achip), chan(achan) { }

  bool isValid() const { return chan < 999; }

  Index chip =0;
  Index chan =999;

};

bool operator<(const AdcChannelId& lhs, const AdcChannelId& rhs);
bool operator==(const AdcChannelId& lhs, const AdcChannelId& rhs);

#endif
