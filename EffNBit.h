// EffNBit.h

// David Adams
// February 2017

#ifndef EffNBit_H
#define EffNBit_H

class EffNBit {

public:

  // Ctor.
  //   again - ADC gain = change input that increases code by one
  //   anbit - Actual number of ADC bits
  EffNBit(double again =0.328, unsigned int anbit =12);

  // Return the resolution of a bin.
  double sig0() const;

  // Return the effective # bits for an input resolution.
  double effnbit(double asig) const;

  // Return the resolution for a given effective # bits.
  double sig(double aeffnbit) const;

  unsigned int nbit;
  double gain;

};

#endif
