// EffNBit.cxx

#include "EffNBit.h"

//**********************************************************************

EffNBit::EffNBit(double again, unsigned int anbit)
: nbit(anbit), gain(again) { }

//**********************************************************************

double EffNBit::sig0() const {
  return gain/sqrt(12.0);
}

//**********************************************************************

double EffNBit::effnbit(double asig) const {
  return double(nbit) - log(asig/sig0())/log(2);
}

//**********************************************************************

double EffNBit::sig(double aeffnbit) const {
  return sig0()*pow(2.0, (double(nbit)-aeffnbit));
}

//**********************************************************************
