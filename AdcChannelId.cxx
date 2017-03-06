// AdcChannelId.cxx

#include "AdcChannelId.h"

bool operator<(const AdcChannelId& lhs, const AdcChannelId& rhs) {
  if ( lhs.chip < rhs.chip ) return  true;
  if ( lhs.chip > rhs.chip ) return false;
  return lhs.chan < rhs.chan;
}
