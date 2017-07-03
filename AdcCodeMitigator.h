// AdcCodeMitigator.h

#ifndef AdcCodeMitigator_H
#define AdcCodeMitigator_H

#include "AdcTypes.h"

class AdcCodeMitigator {

public:

  virtual ~AdcCodeMitigator() = default;

  virtual AdcCode mitigatedCode(AdcCode code, SampleIndex isam) const = 0;

};

#endif
