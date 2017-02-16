// histNSigma.h

#include "TH1.h"

// Return the part of a histogram that is outside of N-sigma.
//   ph - input histogram
//   nsig - range for selection
//   nrange - if >= than zero, user range is nrange bins around the nonzero
//            region of the input plot.

TH1* histNSigma(TH1* ph, double nsig, int nrange =0);
