// AdcChipStatus.h

// Returns if a chip is good or bad.

#ifndef AdcChipStatus_H
#define AdcChipStatus_H

class AdcChipStatus {

public:

  using Index = unsigned int ;

  bool isGood(Index chip) const;
  bool isBad(Index chip) const;
  bool isKnown(Index chip) const;

};

#endif
