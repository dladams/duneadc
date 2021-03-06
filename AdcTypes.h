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
using AdcTime = Index;   // 4 bytes covers 1970-2106
using Float = float;

using SampleVector = std::vector<SampleValue>;
using SampleVector = std::vector<SampleValue>;
using ShortIndexVector = std::vector<ShortIndex>;
using IndexVector = std::vector<Index>;
using SampleIndexVector = std::vector<SampleIndex>;
using FloatVector = std::vector<float>;

inline Index badChannel() { return Index(-1); }
inline Index badChip() { return Index(-1); }
inline Index badTime() { return AdcTime(-1); }
inline Index badIndex() { return Index(-1); }
inline ShortIndex badShortIndex() { return ShortIndex(-1); }

#endif
