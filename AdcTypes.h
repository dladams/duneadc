// AdcTypes

// David Adams
// May 2017

// Aliases for simple types used in duneadc.

#ifndef AdcTypes_H
#define AdcTypes_H

#include <vector>

using AdcCode = unsigned short;
using SampleValue = AdcCode;
using Index = unsigned int;
using SampleIndex = unsigned long;
using AdcTime = unsigned long;
using SampleVector = std::vector<SampleValue>;
using SampleVector = std::vector<SampleValue>;
using SampleIndexVector = std::vector<SampleIndex>;

inline Index badChannel() { return Index(-1); }
inline Index badChip() { return Index(-1); }

#endif
