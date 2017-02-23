// AdcChipStatus.cxx

#include "AdcChipStatus.h"
#include <set>

using std::set;

//**********************************************************************

AdcChipStatus* AdcChipStatus::instance() {
  static AdcChipStatus* pobj = new AdcChipStatus;
  return pobj;
}

//**********************************************************************

bool AdcChipStatus::good(Index chip) {
  return instance()->isGood(chip);
}

//**********************************************************************

bool AdcChipStatus::isKnown(Index chip) const {
  set<Index> knownChips = { 0,      2,  3,  4,  5,  6,  7,  8,
                           10,             14, 15, 16, 17, 18,
                               21, 22,     24, 25, 26,         29,
                           30, 31, 32, 33,     35};
  return knownChips.find(chip) != knownChips.end();
}

//**********************************************************************

bool AdcChipStatus::isGood(Index chip) const {
  return isKnown(chip) && !isBad(chip);
}

//**********************************************************************

bool AdcChipStatus::isBad(Index chip) const {
  set<Index> badChips = {         2,  3,          6,  7,
                                           14, 15,     17, 18,
                               21,             25,             29,
                                       33,     35};
  return badChips.find(chip) != badChips.end();
}

//**********************************************************************
