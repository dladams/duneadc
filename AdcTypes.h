// AdcTypes

// David Adams
// May 2017

// Aliases for simple types used in duneadc.

#ifndef AdcTypes_H
#define AdcTypes_H

#include <vector>

using ShortIndex = unsigned short;
using AdcCode = unsigned short;
using SampleValue = AdcCode;
using Index = unsigned int;
using SampleIndex = unsigned long;
using AdcTime = unsigned int;   // 4 bytes covers 1970-2106
using SampleVector = std::vector<SampleValue>;
using SampleVector = std::vector<SampleValue>;
using ShortIndexVector = std::vector<ShortIndex>;
using SampleIndexVector = std::vector<SampleIndex>;

inline Index badChannel() { return Index(-1); }
inline Index badChip() { return Index(-1); }

#endif
